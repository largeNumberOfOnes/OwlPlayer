#include "audioFile.h"

#include "had_logger.h"
#include "had_types.h"
#include "had_audio.h"

#include <spa/param/audio/format-utils.h>
#include "pipewire/pipewire.h"
#include "pipewire/stream.h"
#include <spa/param/props.h>

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

        size_t samples_read;
        ret = data.audio_file->read_file(dst, data.audio_file->byte_to_samples(spa_buf->datas[0].maxsize), samples_read);
        if (ret == AudioFile::res_code::end_of_file) {
            *data.state = State::inited;
            *data.was_finalized_val = true;
            spa_buf->datas[0].chunk->size = 0;
            pw_stream_queue_buffer(data.stream, buffer);
            pw_stream_flush(data.stream, true);
            pw_thread_loop_unlock(data.loop);
            return;
        }
        spa_buf->datas[0].chunk->size = data.audio_file->samples_to_byte(samples_read);

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
                // DEV [Must depend on audio_file] // if mpg123 is used
            // .format = SPA_AUDIO_FORMAT_S32,
                // DEV [Must depend on audio_file] // if sndfile is used
            .rate     = static_cast<uint32_t>(audio_file.get_rate()),
            .channels = static_cast<uint32_t>(audio_file.get_channels())
        );

        // std::cout << "rate " << audio_file.get_rate() << std::endl;
        // std::cout << "chan " << audio_file.get_channels() << std::endl;
        
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

    // res Audio::drop();
    // res Audio::run();
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
        return audio_file.samples_to_seconds(audio_file.get_cur_pos());
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

    std::vector<std::complex<float>> Audio::get_samples() {
        afwaf
    }

    bool Audio::can_be_played(std::string_view path) {
        return path.ends_with(".mp3"); // DEV [should check file]
    }
}
