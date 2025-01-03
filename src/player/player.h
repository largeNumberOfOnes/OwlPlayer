#pragma once

#include <string>

#include "abstract.h"

class Player {
    private:
        volume vol = 0;

        void load(std::string file_path);
        void play();
        void forward(seconds step);
        void backward(seconds step);
        void jump(seconds pos);
        void set_volume(volume vol);
        volume get_volume() const;

    public:
        void push_command();
};
