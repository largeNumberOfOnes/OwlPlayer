



#include "setup.h"
#include "had/had.h"



std::string Setup::get_default_file_dir() {
    return default_file_dir;
}

int Setup::get_fps() {
    return fps;
}


enum class Command {
    map,
    set,
};

void parse_string(std::string str) {
    Command command;

    int sep = str.find(' ');
    if (sep == str.length()) {

    }
    // std::string command_str = str.substr(0, str.)
    // if (str.
}


had::Res Setup::read_config(std::string path) {





    return had::Res::success;
}
