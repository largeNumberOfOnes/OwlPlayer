#pragma once

#include <vector>
#include <string>

#include "../glob.h"

//? #######################################################################

enum RUN_MODE {
    JUST_PLAY,
    VERSION,
    ERROR,
    HELP,
    RUN,
};

struct Options {
    // pass
};

RUN_MODE read_args(
    int argc,
    char const *const *argv,
    std::vector<std::string> &files,
    Options &options
);

glob::RET print_version();
glob::RET print_help();
