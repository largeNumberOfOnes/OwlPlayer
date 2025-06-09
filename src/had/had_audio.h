
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

        enum class res_code {
            success,
            // pass
        };

        public:
            Audio(res& result, const Logger& log);
            ~Audio();

            res load(std::string path);
            res drop();
            res run();
            res stop();
            res play();

            res set_volume(volume vol);
            res jump(seconds pos);
            res jump_rel(seconds pos_rel);
    };

    // res aud_set_volume(volume vol);
    // volume aud_get_volume();
    // res aud_set_cur_pos(seconds pos);
    // seconds aud_get_cur_pos();
    // seconds aud_get_duration();
};
