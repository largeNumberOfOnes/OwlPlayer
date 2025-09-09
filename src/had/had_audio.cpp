#include "audioFile.h"

#include "had_logger.h"
#include "had_types.h"
#include "had_audio.h"

#include <chrono>
#include <cstddef>
#include <cstring>
#include <memory>
#include <spa/param/audio/format-utils.h>
#include "pipewire/pipewire.h"
#include "pipewire/stream.h"
#include <spa/param/props.h>

#include <string>
#include <string_view>
#include <complex>
#include <mutex>
#include <cmath>



namespace had {
    Audio::Audio(Res& result, const Logger& log)
        : audio_file(log)
        , log(log)
    {
        data.audio_file = &audio_file;
        data.mutex = &mutex;
        data.state = &state;
        data.was_finalized_val = &was_finalized_val;
        data.period_buf = &period_buf;
        pw_init(nullptr, nullptr);

        data.loop = pw_thread_loop_new("audio_player", nullptr);
        if (!data.loop) {
            log.log_err("Cannot create pw_thread_loop");
            goto on_error;
        }

        data.stream = pw_stream_new_simple(
            pw_thread_loop_get_loop(data.loop),
            "owl_player", // DEV [change stream name]
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

        pw_thread_loop_start(data.loop);

        on_error:
            result = Res::error;
    }

    Audio::~Audio() {
        if (is_stream_connected) {
            pw_stream_disconnect(data.stream);
            is_stream_connected = false;
        }
        pw_stream_destroy(data.stream);
        pw_thread_loop_stop(data.loop);
        pw_thread_loop_destroy(data.loop);
        pw_deinit();
    }

    Res Audio::set_params(const AudioProperties& props) {
        if (is_stream_connected) {
            pw_stream_disconnect(data.stream);
        }
        return Res::success;
    }

    void Audio::on_process(void* userdata) {
        Data_pw& data = *static_cast<Data_pw*>(userdata);
        AudioFile::res_code ret;
        std::lock_guard<std::mutex> lock{*data.mutex};
        pw_thread_loop_lock(data.loop);
        if (*data.state != State::playing) {
            pw_thread_loop_unlock(data.loop);
            return;
        }
        struct pw_buffer* buffer;
        struct spa_buffer* spa_buf;

        buffer = pw_stream_dequeue_buffer(data.stream);
        if (buffer == NULL) {
            pw_log_warn("Out of buffers");
            return;
        }

        spa_buf = buffer->buffer;
        Value* dst = static_cast<Value*>(spa_buf->datas[0].data);

        if (dst == NULL) {
            return;
        }

        size_t samples_read = 0;
        std::size_t buf_byte_size = spa_buf->datas[0].maxsize;
        SampleDem buf_samples_size =
            data.audio_file->byte_to_samples(buf_byte_size);
        data.period_buf->resize(
            buf_samples_size * data.audio_file->get_channels()
        );
        data.period_buf->get_ptr()[243] = 32;
        ret = data.audio_file->read_file(
            data.period_buf->get_ptr(),
            buf_samples_size,
            samples_read
        );
        data.period_buf->set_elem_count(
            samples_read  * data.audio_file->get_channels()
        );
        data.period_buf->reset_step();
        std::memcpy(dst, data.period_buf->get_ptr(), buf_byte_size);
        if (ret == AudioFile::res_code::end_of_file) {
            *data.state = State::inited;
            *data.was_finalized_val = true;
            spa_buf->datas[0].chunk->size = 0;
            pw_stream_queue_buffer(data.stream, buffer);
            pw_stream_flush(data.stream, true);
            pw_thread_loop_unlock(data.loop);
            return;
        }
        spa_buf->datas[0].chunk->size =
                            data.audio_file->samples_to_byte(samples_read);

        pw_stream_queue_buffer(data.stream, buffer);

        pw_thread_loop_unlock(data.loop);
    }

    Audio::res_code Audio::drop() {
        pw_thread_loop_lock(data.loop);
        if (is_stream_connected) {
            pw_stream_disconnect(data.stream);
            is_stream_connected = false;
        }
        pw_thread_loop_unlock(data.loop);
        return res_code::success;
    }

    Audio::res_code Audio::load(std::string_view path) {
        std::lock_guard<std::mutex> lock{mutex};
        audio_file.load(path);
        pw_thread_loop_lock(data.loop);
        if (is_stream_connected) {
            pw_stream_disconnect(data.stream);
            is_stream_connected = false;
        }

        uint8_t buffer[1024];
        struct spa_pod_builder builder =
                            SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));
        spa_audio_info_raw audio_info = SPA_AUDIO_INFO_RAW_INIT(
            .format = SPA_AUDIO_FORMAT_S16,
            .rate     = static_cast<uint32_t>(audio_file.get_rate()),
            .channels = static_cast<uint32_t>(audio_file.get_channels())
        );

        const struct spa_pod *params[1] = {
            spa_format_audio_raw_build(
                &builder,
                SPA_PARAM_EnumFormat,
                &audio_info
            )
        };
        if (!params[0]) {
            drop();
            return res_code::other_error;
        }
        
        int err = pw_stream_connect(data.stream,
                          PW_DIRECTION_OUTPUT,
                          PW_ID_ANY,
                          static_cast<pw_stream_flags>(
                            PW_STREAM_FLAG_AUTOCONNECT |
                            PW_STREAM_FLAG_MAP_BUFFERS |
                            PW_STREAM_FLAG_RT_PROCESS
                          ),
                          params, 1);
        if (err) {
            drop();
            return res_code::other_error;
        }
        is_stream_connected = true;
        pw_stream_set_active(data.stream, false);
        state = State::stoped;
        
        set_volume_unsafe();

        pw_thread_loop_unlock(data.loop);
        
        return res_code::success;
    }

    Audio::res_code Audio::stop() {
        if (state != State::playing) {
            return res_code::bad_state;
        }
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        pw_stream_flush(data.stream, true);
        pw_stream_set_active(data.stream, false);
        pw_thread_loop_unlock(data.loop);
        state = State::stoped;
        return res_code::success;
    }
    Audio::res_code Audio::play() {
        if (state != State::stoped) {
            return res_code::bad_state;
        }
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        pw_stream_set_active(data.stream, true);
        was_finalized_val = false;
        pw_thread_loop_unlock(data.loop);
        state = State::playing;
        return res_code::success;
    }

    Res Audio::set_volume_unsafe() {
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
        return Res::success;
    }

    Audio::res_code Audio::set_volume(Volume vol) {
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        this->vol = vol;
        Res res = set_volume_unsafe();
        pw_thread_loop_unlock(data.loop);
        return (res == Res::success) ? res_code::success
                                                : res_code::other_error;
    }

    Volume Audio::get_volume() {
        return vol;
    }

    Audio::res_code Audio::jump(seconds pos) {
        if (state != State::stoped && state != State::playing) {
            return res_code::bad_state;
        }
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        pw_stream_flush(data.stream, false);
        std::size_t samples_pos = audio_file.seconds_to_samples(pos);
        samples_pos = std::min(
            samples_pos,
            audio_file.get_max_pos()
        );
        AudioFile::res_code res = audio_file.set_position(samples_pos);
        pw_thread_loop_unlock(data.loop);
        if (res == AudioFile::res_code::success) {
            return res_code::success;
        } else {
            return res_code::other_error;
        }
    }

    Audio::res_code Audio::jump_rel(seconds time_shift) {
        if (state != State::stoped && state != State::playing) {
            return res_code::bad_state;
        }
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        pw_stream_flush(data.stream, false);
        bool is_positive = time_shift > 0;
        std::size_t samples_pos = audio_file.seconds_to_samples(std::abs(time_shift));
        if (is_positive) {
            samples_pos = audio_file.get_cur_pos() + samples_pos;
        } else {
            if (audio_file.get_cur_pos() < samples_pos) {
                samples_pos = 0;
            } else {
                samples_pos = audio_file.get_cur_pos() - samples_pos;
            }
        }
        samples_pos = std::min(
            samples_pos,
            audio_file.get_max_pos()
        );
        audio_file.set_position(samples_pos);
        pw_thread_loop_unlock(data.loop);
        return res_code::success;
    }

    bool Audio::is_playing() {
        return state == State::playing;
    }

    bool Audio::is_stoped() {
        return state == State::stoped;
    }

    had::seconds Audio::get_cur_time() {
        if (state == State::inited) {
            return 0;
        }
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        struct pw_time time;
        uint64_t now = pw_stream_get_time_n(data.stream, &time, sizeof(time));
        // now_ns = pw_stream_get_nsec(data.stream);
        // int64_t ret = time.ticks;
        // int64_t now = pw_stream_get_nsec(data.stream);
        // struct spa_system *system = pw_loop_get_system(data.loop);
        // int64_t now = spa_system_get_nsec(system);
        // DEV [necessary to update pipewire to unblock usage of
        // DEV         pw_stream_get_nsec function()]

        static int counter = 0;
        ++counter;
        log.log_info(
            std::to_string(counter) + " " +
            // std::to_string(1000 * time.queued / audio_file.get_rate()) + " " +
            std::to_string(time.now) + " " +
            std::to_string(now) + " " +
            std::to_string(
                audio_file.samples_to_seconds(audio_file.get_cur_pos())
            )
        );
        pw_thread_loop_unlock(data.loop);
        return audio_file.samples_to_seconds(audio_file.get_cur_pos());
        // return ret;
    }

    had::seconds Audio::get_duration() {
        if (state != State::playing && state != State::stoped) {
            return 0;
        }
        return audio_file.get_samples() / audio_file.get_rate();
    }

    bool Audio::was_finalized() {
        if (was_finalized_val) {
            was_finalized_val = false;
            return true;
        } else {
            return false;
        }
    }

    bool Audio::can_be_played(std::string_view path) {
        return path.ends_with(".mp3"); // DEV [should check file]
    }

    void Audio::get_samples(std::vector<std::complex<float>>& ret) {

    }

    void Audio::Buffer::set_elem_count(std::size_t elem_count) {
        this->elem_count = elem_count;
    }

    void Audio::Buffer::resize(std::size_t new_size) {
        if (new_size != size) {
            // buf.reset(new Value[new_size]);
            buf = std::make_unique<Value*>(new Value[new_size]{7});
            size = new_size;
        }
    }

    Value* Audio::Buffer::get_ptr() {
        return *buf;
    }

    std::size_t Audio::Buffer::get_size() {
        return size;
    }

    int Audio::Buffer::get_step() {
        return buf_step;
    }

    void Audio::Buffer::set_frames_per_period(int frames_count) {
        frames_per_period = frames_count;
    }

    int Audio::Buffer::get_frames_per_period() {
        return frames_per_period;
    }

    std::size_t Audio::Buffer::get_elem_count() {
        return elem_count;
    }

    void Audio::Buffer::reset_step() {
        buf_step = 0;
    }
    
    void Audio::Buffer::inc_step() {
        if (buf_step + 1 < frames_per_period) {
            ++buf_step;
        }
    }

    void Audio::set_frame_per_period(int frame_count) {
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        period_buf.set_frames_per_period(frame_count);
        pw_thread_loop_unlock(data.loop);
    }

    SampleDem Audio::get_samples_on_period() {
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);
        SampleDem ret = period_buf.get_size() / sizeof(Value);
        pw_thread_loop_unlock(data.loop);
        return ret;
    }

    std::vector<
        std::vector<std::complex<float>>
    > Audio::get_frame() {
        std::lock_guard<std::mutex> lock{mutex};
        pw_thread_loop_lock(data.loop);

        int count = std::min(
            period_buf.get_size() / period_buf.get_frames_per_period(),
            period_buf.get_elem_count()
        );
        int offset = period_buf.get_step() * count;
        int channels = audio_file.get_channels();
        std::vector<
            std::vector<std::complex<float>>
        > ret;
        for (int ch = 0; ch < channels; ++ch) {
            ret.push_back(std::vector<std::complex<float>>{});
        }
        for (int q = 0; q < count; ++q) {
            for (int ch = 0; ch < channels; ++ch) {
                ret[ch].push_back(
                    std::complex<float>(
                        period_buf.get_ptr()[offset + q * channels + ch]
                    )
                );
            }
        }
        if (state == State::playing) {
            period_buf.inc_step();
        }

        pw_thread_loop_unlock(data.loop);
        return ret;
    }
}
