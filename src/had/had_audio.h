/**
    This class represent interface to load and play audio files
 */

#pragma once

#include "audioFile.h"
#include "had_types.h"
#include "had_logger.h"

#include "pipewire/pipewire.h"

#include <functional>
#include <mutex>
#include <string>
#include <complex>
#include <string_view>



namespace had {
    class Audio {
        AudioFile audio_file;
        mutable std::mutex mutex;

        Volume vol = 100;

        bool is_stream_connected = false;
        bool was_finalized_val = false;
        enum class State {
            inited,
            stop,
            play,
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
        // static void on_drain(void* userdata);
        struct pw_stream_events stream_events = {
            .process = on_process,
            // .drained = on_drain,
        };
        struct AudioProperties {
            int rate;
            int channels;
            int samples;
        };

        Res set_params(const AudioProperties& props);

        std::size_t pos_to_byte(seconds pos);
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
            bool was_finalized();

            had::seconds get_cur_time();
            had::seconds get_duration();
            void get_samples(std::vector<std::complex<float>>& ret);

            res_code set_volume(Volume vol);
            Volume get_volume();
            res_code jump(seconds pos);
            res_code jump_rel(seconds pos_rel);

            static bool can_be_played(std::string_view path);
    };
};
