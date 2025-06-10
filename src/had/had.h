/*
 * This is an interface that implements all platform-dependent functions.
 * Implementation of these functions can be overridden externally, so it
 *    is assembled to an object file separately from the rest.
 */

#pragma once


#include "had_keys.h"
#include "had_interface.h"









// #include <cstddef>
// #include <string>
// #include <iostream>
// #include "../glob_types.h"

// namespace had {
    // Cathing symbols
    // key get_key();

    // Interface
    // res start_window();
    // res end_window();
    // void set_color();
    // void move();
    // res draw_slider(dem x, dem y, dem len, dem val);
    // res draw_text(dem x, dem y, char const* str);
    // res draw_symbol(dem x, dem y, char ch);
    // res draw_wide_symbol(dem x, dem y, wchar_t ch);
    // res update();
    // res cls();
    // dem get_width(); // On error returns 0
    // dem get_height(); // On error returns 0

    // Color
    // using Color = int;
    // using ColorPair = int;
    // ColorPair const default_color_pair = 0;
    // namespace Colors {
    //     extern Color const black;
    //     extern Color const red;
    //     extern Color const green;
    //     extern Color const yellow;
    //     extern Color const blue;
    //     extern Color const magenta;
    //     extern Color const cyan;
    //     extern Color const white;
    // }

    // res set_new_color(Color& col, int r, int g, int b);
    // res change_color(Color col, int r, int g, int b);

    // res set_new_color_pair(ColorPair& pair, Color t, ColorPair b);
    // res change_color_pair(ColorPair pair, Color t, ColorPair b);
    // res use_color_pair(ColorPair pair);
    // res use_no_color_pair();

    // AudioPlayer


    // std::string get_system_config_path();
// };
