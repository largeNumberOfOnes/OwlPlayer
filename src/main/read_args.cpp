#include <string>
#include <vector>

#include "modes.h"

//? #######################################################################

RUN_MODE read_args(
    int argc,
    char const *const *argv,
    std::vector<std::string> &files
) {

    for (int q = 0; q < argc; ++q) {

        std::string arg{argv[q]};
        if (arg.empty())
            return RUN_MODE::ERROR;

        if (arg[0] == '-') {
            if (arg == "-h" || arg == "--help")
                return RUN_MODE::HELP;
            if (arg == "--version")
                return RUN_MODE::VERSION;
            if (arg == "-just_play")
                return RUN_MODE::JUST_PLAY;
        } else {
            files.push_back(arg);
        }

    }

    return RUN_MODE::ERROR;
}

// bool read_options_test() {

//     const char * argv[] = {
//         "-h"
//     };
//     const char *const argv[] = {
//         "--help"
//     };

//     // int options_count;
//     // read_options(
//     //     argc, argv,
//     //     options_count,
//     // );

// }
