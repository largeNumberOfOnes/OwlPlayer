#pragma once

#include <cstddef>
#include <string>
#include <iostream>
#include "glob_types.h"




namespace had {

    // Base types
    using dem = unsigned int;
    using seconds = unsigned int;
    using volume = unsigned int;

    // Cathing symbols
    enum class key {
        arrow_rigth,
        arrow_left,
        arrow_up,
        arrow_down,
        space,
        q,
        nothing,
    };
    key get_key();

    // Interface
    res start_window();
    res end_window();
    void set_color();
    void move();
    res draw_slider(dem x, dem y, dem len, dem val);
    res draw_text(dem x, dem y, char const* str);
    res draw_symbol(dem x, dem y, char ch);
    res draw_wide_symbol(dem x, dem y, wchar_t ch);
    res update();
    res cls();
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

    res set_new_color(Color& col, int r, int g, int b);
    res change_color(Color col, int r, int g, int b);

    res set_new_color_pair(ColorPair& pair, Color t, ColorPair b);
    res change_color_pair(ColorPair pair, Color t, ColorPair b);
    res use_color_pair(ColorPair pair);
    res use_no_color_pair();

    // AudioPlayer
    res aud_start();
    res aud_end();
    res aud_load(char const* path);
    res aud_run();
    res aud_drop();
    res aud_play_stop();
    res aud_set_volume(volume vol);
    volume aud_get_volume();
    res aud_set_cur_pos(seconds pos);
    seconds aud_get_cur_pos();
    seconds aud_get_duration();

    std::string get_system_config_path();
};
