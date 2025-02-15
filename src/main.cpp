/**
 */

// #include "read_args.h"
// #include "load"
#include "glob_types.h"
#include "player/GUIPlayer.h"


int main(int argc, const char** argv) {

    // Context cont;
    // if (false
    //     || parse_args(argc, argv, cont)
    // ) {
    //     log_err("Error while parsing arguments");
    //     return -1;
    // }

    // load config
    // Config conf{cont.config_path()};

    GUIPlayer player{"/home/dt/Documents"};
    if (player.start_loop()) {
        log_err("Error");
        return -1;
    }

    // clear

    return 0;
}
