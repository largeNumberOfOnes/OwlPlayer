/**
 *
 */

#include "glob_types.h"
#include "config/config.h"
#include "GUIPlayer/GUIPlayer.h"

#ifdef USE_WHILE_BARRIER
    #define WHILE_BARRIER {                                               \
        int cond = true;                                                  \
        while (cond) {}                                                   \
    }
#else
    #define WHILE_BARRIER
#endif



int main(int argc, const char** argv) {

    Config conf{argc, argv};

    WHILE_BARRIER;

    if (conf.run_GUIPlayer()) {
        GUIPlayer player{conf.get_path()};
        if (player.start_loop()) {
            log_err("Error");
            return -1;
        }
    }

    return 0;
}
