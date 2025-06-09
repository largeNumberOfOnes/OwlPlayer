




#include "audioFile.h"
#include "had_types.h"
#include "had_audio.h"
#include "had_logger.h"

#include "pipewire/pipewire.h"
#include "spa/param/audio/raw.h"
#include <spa/param/audio/format-utils.h>
#include <spa/param/props.h>

#include <mutex>

namespace had {

    Audio::Audio(res& result, const Logger& log)
        : audio_file(log)
    {
        data.audio_file = &audio_file;
        data.mutex = &mutex;
        pw_init(nullptr, nullptr);

        data.loop = pw_thread_loop_new("audio_player", nullptr);
        if (!data.loop) {
            log.log_err("Cannot create pw_thread_loop");
            goto on_error;
        }

        data.stream = pw_stream_new_simple(
            pw_thread_loop_get_loop(data.loop),
            "audio_player_stream_simple", // DEV [change stream name]
            pw_properties_new(
                PW_KEY_MEDIA_TYPE,     "Audio",
                PW_KEY_MEDIA_CATEGORY, "Playback",
                PW_KEY_MEDIA_ROLE,     "Music",
                NULL
            ),
            &stream_events,
            &data
        );
        if (!data.stream) {
            log.log_err("Cannot create pw_stream");
            goto on_error;
        }

        // uint8_t buffer[1024];
        // struct spa_pod_builder builder = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));
        // spa_audio_info_raw audio_info = SPA_AUDIO_INFO_RAW_INIT(
        //     .format = SPA_AUDIO_FORMAT_S16,
        //     .rate = static_cast<uint32_t>(audioFile.get_rate()),
        //     .channels = static_cast<uint32_t>(audioFile.get_channels())
        // );

        // const struct spa_pod *params[1] = {
        //     spa_format_audio_raw_build(
        //         &builder,
        //         SPA_PARAM_EnumFormat,
        //         &audio_info
        //     )
        // };

        // pw_stream_connect(
        //     data.stream,
        //     PW_DIRECTION_OUTPUT,
        //     PW_ID_ANY,
        //     (pw_stream_flags)(
        //         PW_STREAM_FLAG_AUTOCONNECT
        //         | PW_STREAM_FLAG_MAP_BUFFERS
        //         | PW_STREAM_FLAG_RT_PROCESS
        //     ),
        //     nullptr, 0
        // );

        pw_thread_loop_start(data.loop);

        on_error:
            result = res::error;
            // pass
    }

    Audio::~Audio() {
        if (is_stream_connected) {
            pw_stream_disconnect(data.stream);
            is_stream_connected = false;
        }
        // pw_stream_destroy(data.stream);
        // pw_thread_loop_stop(data.loop);
        // pw_thread_loop_destroy(data.loop);
        pw_deinit();
    }

    res Audio::set_params(const AudioProperties& props) {
        if (is_stream_connected) {
            pw_stream_disconnect(data.stream);
        }
        return res::success;
    }

    void Audio::on_process(void* userdata) {
        Data_pw& data = *static_cast<Data_pw*>(userdata);

        std::lock_guard<std::mutex> lock{*data.mutex};
        
        struct pw_buffer* buffer;
        struct spa_buffer* spa_buf;
        float* dst;

        buffer = pw_stream_dequeue_buffer(data.stream);
        if (buffer == NULL) {
            pw_log_warn("Out of buffers");
            return;
        }

        spa_buf = buffer->buffer;
        dst = static_cast<float*>(spa_buf->datas[0].data);

        if (dst == NULL) {
            return;
        }

    //     // Read audio data from the file
        // size_t bytes_read =
                //  fread(dst, 1, spa_buf->datas[0].maxsize, data->file);
        size_t bytes_read;
        // data.audio_file->read_file(dst, spa_buf->datas[0].maxsize/sizeof(short), bytes_read);
        data.audio_file->read_file(dst, spa_buf->datas[0].maxsize, bytes_read);
        if (bytes_read == 0) {
            // End of file, stop the main loop
            // pw_main_loop_quit(data.loop);
            return;
        }

        spa_buf->datas[0].chunk->size = bytes_read;

        pw_stream_queue_buffer(data.stream, buffer);
    }

    res Audio::load(std::string path) {

        // if (state == State::inited) 
        std::lock_guard<std::mutex> lock{mutex};

        audio_file.load(path);

        pw_thread_loop_lock(data.loop);

        if (is_stream_connected) {
            pw_stream_disconnect(data.stream);
            is_stream_connected = false;
        }

        uint8_t buffer[1024];
        struct spa_pod_builder builder = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));
        spa_audio_info_raw audio_info = SPA_AUDIO_INFO_RAW_INIT(
            .format = SPA_AUDIO_FORMAT_S16, // DEV [Must depend on audio_file] // if mpg123 lib is used
            // .format = SPA_AUDIO_FORMAT_S32, // DEV [Must depend on audio_file] // if sndfile lib is used
            .rate = static_cast<uint32_t>(audio_file.get_rate()),
            .channels = static_cast<uint32_t>(audio_file.get_channels())
        );

        std::cout << "rate " << audio_file.get_rate() << std::endl;
        std::cout << "chan " << audio_file.get_channels() << std::endl;

        const struct spa_pod *params[1] = {
            spa_format_audio_raw_build(
                &builder,
                SPA_PARAM_EnumFormat,
                &audio_info
            )
        };

        pw_stream_connect(data.stream,
                          PW_DIRECTION_OUTPUT,
                          PW_ID_ANY,
                          static_cast<pw_stream_flags>(
                            PW_STREAM_FLAG_AUTOCONNECT |
                            PW_STREAM_FLAG_MAP_BUFFERS |
                            PW_STREAM_FLAG_RT_PROCESS
                          ),
                          params, 1);
        is_stream_connected = true;

        pw_thread_loop_unlock(data.loop);
        
        return res::success;
    }

    // res Audio::drop();
    // res Audio::run();
    res Audio::stop() {
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        pw_stream_set_active(data.stream, false);
        pw_thread_loop_unlock(data.loop);
        return res::success;
    }
    res Audio::play() {
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        pw_stream_set_active(data.stream, true);
        pw_thread_loop_unlock(data.loop);
        return res::success;
    }

    res Audio::set_volume(volume vol) {
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        std::size_t channels = audio_file.get_channels();
        float* volumes = new float[channels];
        for (long q = 0; q < channels; q++) {
            volumes[q] = (float)vol/100;
        }
        pw_stream_set_control(
            data.stream,
            SPA_PROP_channelVolumes,
            channels,
            volumes,
            0
        );
        delete[] volumes;
        pw_thread_loop_unlock(data.loop);
        return res::success;
    }

    res Audio::jump(seconds pos) {
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        pw_stream_flush(data.stream, false);
        std::size_t samples_pos = pos * audio_file.get_rate();
        samples_pos = std::min(
            samples_pos,
            static_cast<std::size_t>(audio_file.get_samples())
        );
        audio_file.set_position(samples_pos);
        pw_thread_loop_unlock(data.loop);
        return res::success;
    }
    
    res Audio::jump_rel(seconds pos_rel) {
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        pw_stream_flush(data.stream, false);
        std::size_t samples_pos = pos_rel * audio_file.get_rate();
        samples_pos = audio_file.get_cur_pos() + samples_pos;
        samples_pos = std::max(0lu, samples_pos);
        samples_pos = std::min(
            samples_pos,
            static_cast<std::size_t>(audio_file.get_samples())
        );
        audio_file.set_position(samples_pos);
        pw_thread_loop_unlock(data.loop);
        return res::success;
    }

}
