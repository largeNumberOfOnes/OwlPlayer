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
        space,
        q,
        nothing,
    };

    res start_window();
    res end_window();

    // Cathing symbols
    key get_key();

    // Interface
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
    res audio_device_ctor();
    res audio_device_dtor();
    res load(char const* path);
    res remove();
    res play_stop_audio();
    res player_update();
    seconds get_duration();
    seconds get_cur_time();
    res jump(had::seconds pos);

    std::string get_system_config_path();
};
