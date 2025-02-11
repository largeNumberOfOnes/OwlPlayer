#pragma once

#include <string>
#include <iostream>

#include "glob_types.h"



namespace had {

    using dem = unsigned int;
    using seconds = unsigned int;
    using volume = unsigned int;

    enum class key {
        arrow_rigth,
        arrow_left,
        arrow_up,
        arrow_down,
        nothing,
    };

    stat start_window();
    stat end_window();

    // Cathing symbols
    key get_key();

    // Interface
    void set_color();
    void move();
    stat draw_slider(dem x, dem y, dem len, dem val);
    stat draw_text(dem x, dem y, char const* str);
    stat update();
    stat cls();
    dem get_width();
    dem get_height();

    // AudioPlayer
    stat load(char const* path);
    stat remove();
    seconds get_duration();
    seconds get_cur_time();
    void jump(had::seconds pos);

    std::string get_config_path();
};
