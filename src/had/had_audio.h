
#include "audioFile.h"
#include "had_types.h"
#include "had_logger.h"

#include "pipewire/pipewire.h"

#include <functional>
#include <mutex>
#include <string>

namespace had {

    class Audio {

        AudioFile audio_file;
        mutable std::mutex mutex;
        struct Data_pw {
            pw_thread_loop* loop;
            pw_stream*      stream;
            AudioFile*      audio_file;
            std::mutex*     mutex;
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

        res set_params(const AudioProperties& props);

        bool is_stream_connected = false;
        enum class State {
            inited,
            stop,
            play,
        } state;

        std::size_t pos_to_byte(seconds pos);

        public:
            enum class res_code {
                success,
                bad_state,
                other_error,
            };

            Audio(res& result, const Logger& log);
            ~Audio();

            res_code load(std::string path);
            res_code drop();
            res_code stop();
            res_code play();

            res_code set_volume(volume vol);
            res_code jump(seconds pos);
            res_code jump_rel(seconds pos_rel);
    };

    // res aud_set_volume(volume vol);
    // volume aud_get_volume();
    // res aud_set_cur_pos(seconds pos);
    // seconds aud_get_cur_pos();
    // seconds aud_get_duration();
};
