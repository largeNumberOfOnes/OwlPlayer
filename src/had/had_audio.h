/**
 * This class represent interface to load and play audio files.
 *
 * This class must exist in a single instance.
 *
 * This class interacts with another thread, so many of its methods use
 *    synchronization primitives. However, the external interface of this
 *    class is not thread-safe.
 */

#pragma once

#include "had_logger.h"
#include "had_types.h"
#include "audioFile.h"

#include "pipewire/pipewire.h"

#include <string_view>
#include <functional>
#include <complex>
#include <string>
#include <mutex>



namespace had {
    class Audio {
        AudioFile audio_file;
        const Logger& log;
        mutable std::mutex mutex;
        Volume vol = 100;

        bool is_stream_connected = false;
        bool was_finalized_val = false;
        enum class State {
            inited,
            stoped,
            playing,
        } state;
        
        struct Data_pw {
            pw_thread_loop* loop;
            pw_stream*      stream;
            AudioFile*      audio_file;
            std::mutex*     mutex;
            State*          state;
            bool*           was_finalized_val;
        } data;

        static void on_process(void* userdata);
        struct pw_stream_events stream_events = {
            .process = on_process,
        };
        struct AudioProperties {
            int rate;
            int channels;
            int samples;
        };

        Res set_params(const AudioProperties& props);

        Res set_volume_unsafe();

        public:
            enum class res_code {
                success,
                bad_state,
                other_error,
            };

            Audio(Res& result, const Logger& log);
            ~Audio();

            res_code load(std::string_view path);
            res_code drop();
            res_code stop();
            res_code play();
            bool is_playing();
            bool is_stoped();
            bool was_finalized(); // After returning true returns false

            had::seconds get_cur_time();
            had::seconds get_duration();
            
            std::vector<std::complex<float>> get_samples();

            res_code set_volume(Volume vol);
            Volume get_volume();
            res_code jump(seconds pos);
            res_code jump_rel(seconds pos_rel);

            static bool can_be_played(std::string_view path);
    };
};
