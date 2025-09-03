#include "audioFile.h"

#include "had_dataFrame.h"
#include "had_logger.h"
#include "had_types.h"

#include <cstddef>
#include <fcntl.h>

#include <optional>
#include <string_view>
#include <string>
#include <vector>
#include <mutex>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswresample/swresample.h>
    #include <libavutil/log.h>
}



static char const* file_path_pcm = "/tmp/audplay_77777.pcm";

namespace had {

    struct FfmpegData {
        AVFormatContext* fmt_ctx        = nullptr;
        int audio_stream_idx            = -1;
        AVStream* audio_stream          = nullptr;
        AVCodecParameters* codec_params = nullptr;
        AVCodecContext* codec_ctx       = nullptr;
        SwrContext* swr_ctx             = nullptr;
    };

    static Res ff_init_context(
        FfmpegData& data,
        const char* input_file,
        const Logger& log
    ) {
        if (avformat_open_input(
                &data.fmt_ctx,
                input_file,
                nullptr,
                nullptr
            ) < 0
        ) {
            log.log_err("Cannot open file");
            return Res::error;
        }
        if (avformat_find_stream_info(data.fmt_ctx, nullptr) < 0) {
            log.log_err("Cannot find stream info");
            avformat_close_input(&data.fmt_ctx);
            return Res::error;
        }
        return Res::success;
    }

    static Res ff_init_stream(
        FfmpegData& data,
        const Logger& log
    ) {
        data.audio_stream_idx = av_find_best_stream(
            data.fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0
        );
        if (data.audio_stream_idx < 0) {
            log.log_err("No audio stream found");
            avformat_close_input(&data.fmt_ctx);
            return Res::error;
        }
        data.audio_stream = data.fmt_ctx->streams[data.audio_stream_idx];
        return Res::success;
    }

    static Res ff_init_codec(
        FfmpegData& data,
        const Logger& log
    ) {
        data.codec_params = data.audio_stream->codecpar;
        const AVCodec* codec =
                        avcodec_find_decoder(data.codec_params->codec_id);
        if (!codec) {
            log.log_err("Cannot find decoder");
            avformat_close_input(&data.fmt_ctx);
            return Res::error;
        }
        data.codec_ctx = avcodec_alloc_context3(codec);
        if (avcodec_parameters_to_context(
                data.codec_ctx,
                data.audio_stream->codecpar
            ) < 0
        ) {
            log.log_err("Cannot setcodec parameters to stream");
            avcodec_free_context(&data.codec_ctx);
            avformat_close_input(&data.fmt_ctx);
            return Res::error;
        }
        if (avcodec_open2(data.codec_ctx, codec, nullptr) < 0) {
            log.log_err("Error opening codec");
            avcodec_free_context(&data.codec_ctx);
            avformat_close_input(&data.fmt_ctx);
            return Res::error;
        }
        return Res::success;
    }

    static Res ff_init_swr(
        FfmpegData& data,
        const Logger& log
    ) {
        data.swr_ctx = swr_alloc();
        swr_alloc_set_opts2(
            &data.swr_ctx,
            &data.codec_ctx->ch_layout,          // Output layout (stereo)
            AV_SAMPLE_FMT_S16,                   // 16-bit signed PCM
            data.codec_ctx->sample_rate,         // Output sample rate
            &data.codec_ctx->ch_layout,          // Input layout
            data.codec_ctx->sample_fmt,          // Input format
            data.codec_ctx->sample_rate,         // Input sample rate
            0, nullptr
        );
        swr_init(data.swr_ctx);

        return Res::success;
    }
    
    static Res write_to_file(
        FfmpegData& data,
        AVFrame* frame,
        SampleDem& samples,
        FILE* pcm_out,
        const Logger& log
    ) {
        uint8_t* pcm_data = nullptr;
        av_samples_alloc(
            &pcm_data, nullptr, data.codec_ctx->ch_layout.nb_channels,
            frame->nb_samples, AV_SAMPLE_FMT_S16, 0
        );
        samples += frame->nb_samples;

        swr_convert(data.swr_ctx, &pcm_data, frame->nb_samples,
                (const uint8_t**)frame->data, frame->nb_samples);

        fwrite(
            pcm_data,
            1,
            frame->nb_samples * data.codec_ctx->ch_layout.nb_channels * 2,
            pcm_out
        );
        av_freep(&pcm_data);

        return Res::success;
    }

    static Res ff_convert(
        FfmpegData& data,
        const char* output_file,
        SampleDem& samples,
        const Logger& log
    ) {
        FILE* pcm_out  = fopen(output_file, "wb");
        AVPacket* pkt  = av_packet_alloc();
        AVFrame* frame = av_frame_alloc();

        samples = 0;
        while (av_read_frame(data.fmt_ctx, pkt) >= 0) {
            if (pkt->stream_index != data.audio_stream_idx) {
                av_packet_unref(pkt);
                continue;
            }
            if (avcodec_send_packet(data.codec_ctx, pkt) < 0) {
                av_packet_unref(pkt);
                continue;
            }
            while (avcodec_receive_frame(data.codec_ctx, frame) == 0) {
                if (write_to_file(
                        data, 
                        frame, 
                        samples, 
                        pcm_out, 
                        log
                    )
                ) {
                    log.log_info("cannot convert received frame");
                }
            }
            av_packet_unref(pkt);
        }

        fclose(pcm_out);
        av_frame_free(&frame);
        av_packet_free(&pkt);

        return Res::success;
    }

    class FfErrors {
        public:
            char* help_str = nullptr;
            std::vector<std::pair<int, std::string>> errors_list;
            FfErrors() {
                constexpr std::size_t ERROR_BUFFER_SIZE = 1024;
                help_str = new char[ERROR_BUFFER_SIZE];
            }
            ~FfErrors() {
                delete [] help_str;
            }
    };

    static FfErrors& ff_set_error_processor() {
        static FfErrors ff_errors;
        auto ff_custom_log = +[](
            void* ptr, int level, const char* fmt, va_list vl
        ) {
            static std::mutex mut;
            std::lock_guard<std::mutex> lock{mut};
            vsprintf(ff_errors.help_str, fmt, vl);
            std::string str{ff_errors.help_str};
            str.pop_back();
            ff_errors.errors_list.emplace_back(level, std::move(str));
        };
        av_log_set_callback(ff_custom_log);
        return ff_errors;
    }

    static Res ff_check_errors(FfErrors& ff_errors, const Logger& log) { 
        av_log_set_callback(av_log_default_callback);
        Res res = Res::success;
        for (const auto& it : ff_errors.errors_list) {
            if (it.first == AV_LOG_ERROR) {
                log.log_err(it.second);
                res = Res::error;
            } else if constexpr (PRINT_FFMPEG_WARNINGS) {
                log.log_warn(it.second);
            }
        }
        return res;
    }

    Res AudioFile::convert_to_pcm(
        const char* input_file,
        const char* output_file
    ) {
        FfmpegData ffmpeg_data;
        FfErrors& ff_errors = ff_set_error_processor();

        if (false
            || ff_init_context(ffmpeg_data, input_file, log)
            || ff_init_stream (ffmpeg_data, log)
            || ff_init_codec  (ffmpeg_data, log)
            || ff_init_swr    (ffmpeg_data, log)
            || ff_convert     (ffmpeg_data, output_file, samples, log)
        ) {
            log.log_err("Cannot convert file to pcm");
            return Res::error;
        }

        rate     = ffmpeg_data.codec_params->sample_rate;
        channels = ffmpeg_data.codec_params->ch_layout.nb_channels;

        swr_free            (&ffmpeg_data.swr_ctx);
        avcodec_free_context(&ffmpeg_data.codec_ctx);
        avformat_close_input(&ffmpeg_data.fmt_ctx);

        if (ff_check_errors(ff_errors, log)) {
            log.log_err("Internal ffmpeg error");
            return Res::error;
        }

        return Res::success;
    }

    Res AudioFile::load(std::string_view path) {
        log.log_info("init() file");
        erase();
        
        if (convert_to_pcm(path.data(), file_path_pcm)) {
            log.log_err("Cannot conver file to pcm");
            return Res::error;
        }

        file = fopen(file_path_pcm, "rb");
        if (!file) {
            log.log_err(
                std::string("Cannot open file: ") + strerror(errno)
            );
            remove(file_path_pcm);
            return Res::error;
        }

        cur_pos = 0;
        is_end_reached = false;
        is_inited = true;

        return Res::success;
    }

    void AudioFile::erase() {
        log.log_info("Erasing file");

        if (!is_inited) {
            is_end_reached = false;
            return;
        }

        fclose(file);
        file = nullptr;
        remove(file_path_pcm);
        is_inited = false;
        is_end_reached = false;

        return;
    }

    AudioFile::AudioFile(const Logger& log) : log(log) {
        log.log_info("Creating an empty AudioFile");
    }

    void AudioFile::copy_to_buf(short* ref_buf, SampleDem samples) {

        DataFrame::DataType data;
        for (SampleDem q = 0; q < samples; ++q) {
            for (int ch = 0; ch < channels; ++ch) {
                data[ch].push_back(
                    std::complex<float>(ref_buf[samples * channels + ch])
                );
            }
        }
        frame_manager.get_in_frame().push(std::move(data), rate, cur_pos);
    }

    AudioFile::res_code AudioFile::read_file(void* buf, SampleDem samples,
                                                    SampleDem& retcount) {
        if (!is_inited) {
            log.log_err("File is not initialized");
            return res_code::other_error;
        }
        if (!buf) {
            log.log_err("buf is derefering nullptr");
            return res_code::other_error;
        }

        if (is_end_reached) {
            retcount = 0;
            return res_code::end_of_file;
        }
        std::size_t size = fread(buf, 1, samples_to_byte(samples), file);

        if (size == 0) {
            retcount = 0;
            is_end_reached = true;
            return res_code::success;
        }
        cur_pos += byte_to_samples(size);
        retcount = byte_to_samples(size);

        copy_to_buf(reinterpret_cast<short*>(buf), retcount);

        return res_code::success;
    }

    AudioFile::res_code AudioFile::set_position(SampleDem position) {
        if (!is_inited) {
            log.log_err("File is not initialized");
            return res_code::not_initialized;
        }

        if (fseek(file, samples_to_byte(position), SEEK_SET)) {
            is_end_reached = true;
            return res_code::success;
        }
        cur_pos = position;
        is_end_reached = false;

        return res_code::success;
    }

    AudioFile::~AudioFile() {
        log.log_info("Destroying AudioFile");
        if (is_inited) {
            erase();
        }
    }

    SampleDem AudioFile::get_cur_pos() {
        return cur_pos;
    }

    SampleDem AudioFile::get_max_pos() {
        return samples;
    }

    int AudioFile::get_rate() {
        return rate;
    }

    int AudioFile::get_channels() {
        return channels;
    }

    int AudioFile::get_samples() {
        return samples;
    }

    SampleDem AudioFile::byte_to_samples(std::size_t bytes) {
        if (channels == 0) {
            return 0;
        }
        return bytes / sizeof(Value) / channels;
    }

    std::size_t AudioFile::samples_to_byte(SampleDem pos) {
        return pos * sizeof(Value) * channels;
    }

    SampleDem AudioFile::seconds_to_samples(seconds time) {
        return time * rate;
    }

    seconds AudioFile::samples_to_seconds(SampleDem samples) {
        if (rate == 0) {
            return 0;
        }
        return samples / rate;
    }

    std::optional<DataFrame> AudioFile::get_first_frame() {
        return frame_manager.get_out_frame();
    }

    DataFrame AudioFile::get_frame(DataFrame&& frame) {
        return frame_manager.update_out_frame(std::move(frame));
    }

    std::size_t AudioFile::FrameManager::get_id() {
        static std::size_t id_counter = 7;
        
    }

    DataFrame& AudioFile::FrameManager::get_in_frame_ref() {
        switch (out_frame) {
            case OutFrame::FIRST:  return frame2;
            case OutFrame::SECOND: return frame1;
        }
    }

    DataFrame& AudioFile::FrameManager::get_out_frame_ref() {
        switch (out_frame) {
            case OutFrame::FIRST:  return frame1;
            case OutFrame::SECOND: return frame2;
        }
    }

    void AudioFile::FrameManager::swap_out_frame() {
        switch (out_frame) {
            case OutFrame::FIRST:  out_frame = OutFrame::SECOND;
            case OutFrame::SECOND: out_frame = OutFrame::FIRST;
        }
    }

    void AudioFile::FrameManager::return_frame(DataFrame&& frame) {
        if (!is_frame_issued) {
            return;
        }
        get_out_frame_ref() = std::move(frame);
        is_frame_issued = false;
    }

    DataFrame& AudioFile::FrameManager::get_in_frame() {
        return get_in_frame_ref();
    }

    std::optional<DataFrame> AudioFile::FrameManager::get_out_frame() {
        if (is_frame_issued) {
            return std::nullopt;
        }
        return std::move(get_out_frame_ref());
    }

    DataFrame AudioFile::FrameManager::update_out_frame(
        DataFrame&& frame
    ) {
        // if (!is_frame_issued) {
            // Error
        // }
        get_out_frame_ref() = std::move(frame);
        if (is_in_frame_ready) {
            swap_out_frame();
            is_in_frame_ready = false;
        }
        return std::move(get_out_frame_ref());
    }

    void AudioFile::FrameManager::mark_in_frame_as_ready() {
        is_in_frame_ready = true;
    }

}
