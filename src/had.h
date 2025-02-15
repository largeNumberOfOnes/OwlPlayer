#pragma once

#include <cstddef>
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
        q,
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
    stat draw_symbol(dem x, dem y, char ch);
    stat draw_wide_symbol(dem x, dem y, wchar_t ch);
    stat update();
    stat cls();
    dem get_width();
    dem get_height();

    // Color
    using Color = int;
    using ColorPair = int;
    ColorPair const default_color_pair = 0;
    namespace Colors {
        extern Color const black;
        extern Color const red;
        extern Color const green;
        extern Color const yellow;
        extern Color const blue;
        extern Color const magenta;
        extern Color const cyan;
        extern Color const white;
    }

    stat set_new_color(Color& col, int r, int g, int b);
    stat change_color(Color col, int r, int g, int b);

    stat set_new_color_pair(ColorPair& pair, Color t, ColorPair b);
    stat change_color_pair(ColorPair pair, Color t, ColorPair b);
    stat use_color_pair(ColorPair pair);
    stat use_no_color_pair();

    // AudioPlayer
    stat load(char const* path);
    stat remove();
    seconds get_duration();
    seconds get_cur_time();
    stat jump(had::seconds pos);

    std::string get_system_config_path();
};
