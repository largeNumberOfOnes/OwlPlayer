


#pragma once

#include "had/had.h"

#include <string>



class Setup {

    std::string default_file_dir = "/home/dt";

    int fps = 20;

    // std::vector<std::pair<had::Key, App::action>> key_bindings;

    public:
        struct Colors {
            had::Color def;
            had::Color dir;
            had::Color file;
            had::Color dir_selected;
            had::Color file_selected;
        } colors;

        int get_fps();

        std::string get_default_file_dir();

};
