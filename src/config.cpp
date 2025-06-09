#include "config.h"



Config::Config(int argc [[maybe_unused]], char const** argv[[maybe_unused]]) {
    // pass
}

Config::~Config() {
    // pass
}

bool Config::run_GUIPlayer() {
    return true;
}

char const* Config::get_path() {
    return "/home/dt/Documents";
}
