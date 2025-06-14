


#pragma once

#include "had/had.h"
#include "had/had_keys.h"

#include <string>
#include <unordered_map>



class Setup {

    std::string default_file_dir = "/home/dt/Desktop/mus";

    int fps = 20;

    std::unordered_map<std::string, had::KeySequence> key_bindings;


    public:
        had::Res read_config(std::string path);
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
