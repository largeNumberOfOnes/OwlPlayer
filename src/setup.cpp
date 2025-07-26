#include "setup.h"

#include "had/had.h"
#include "had/had_keys.h"



std::string Setup::get_default_file_dir() {
    return default_file_dir;
}

Shuffler::Mode Setup::get_default_shuffler_mode() {
    return default_shuffler_mode;
}

had::KeySequence Setup::get_input_wrapper_exit_seq() {
    return input_wrapper_exit_seq;
}

int Setup::get_fps() const {
    return fps;
}

const Setup::BindingsList& Setup::get_key_bindings() const {
    return key_bindings;
}

had::Res Setup::read_config(std::string path) {

    return had::Res::success;
}
