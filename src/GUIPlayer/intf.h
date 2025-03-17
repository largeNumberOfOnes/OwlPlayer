#pragma once

// #include <iostream>
// #include <string>
// #include <vector>

#include "../glob_types.h"
#include "../had/had.h"



namespace intf {
    // std::string cur_name = "Unknown";
    // Abstract& abs;
    // volume vol = 0;
    // seconds tim = 0;

    // static dem const min_h_to_draw_topPannel = 4;
    // static dem const vol_min_width = 12;
    // static dem const tim_min_width = 26;
    // static dem const vol_tim_pad = 3;
    // static dem const min_width = vol_min_width + tim_min_width +
    //                                                     vol_tim_pad;
    // static dem const draw_top_h_trig = 10;
    // static dem const min_height = 5;
    // static dem const tim_half_width = 40;
    // static dem const vol_max_width = 20;
    // static dem const top_min_width = 10;

    // bool top_data_is_set = false;

    // struct Int_metrics {
    //     dem vol_w = 0;
    //     dem vol_x = 0;
    //     dem vol_y = 0;

    //     dem tim_w = 0;
    //     dem tim_x = 0;
    //     dem tim_y = 0;

    //     dem top_h = 0;
    //     bool top_draw = false;
    // };
    // Int_metrics imet;
    // res calc_vol_tim(dem w, dem h);
    // res calc_top(dem w, dem h);
    // res calc_int_metrics(dem w, dem h);

    // res draw_topPannel(dem w, dem h);

    // res draw_volume(dem x, dem y, dem w);
    // res draw_timeline(dem x, dem y, dem w);

    // dem old_w = 0;
    // dem old_h = 0;


    // // Interface(Abstract& abs);
    // res draw();

    // void set_top_data();

    // volume get_vol();
    // void set_vol(volume vol);

    // volume get_tim();
    // void set_tim(seconds tim);

    // dir_color();

    // struct Color {
    //     int r;
    //     int g;
    //     int b;
    // };

    // Colors
    // extern had::ColorPair color_dir;
    // extern had::ColorPair color_dir_selected;
    // extern had::ColorPair color_file;
    // extern had::ColorPair color_file_selected;

    res init_colors();

    res draw_list_end_symbol(had::dem x, had::dem y);
    res draw_list_symbol(had::dem x, had::dem y);

    res setcol_dir();
    res setcol_file();
    res setcol_dir_selected();
    res setcol_file_selected();
    res setcol_default();
    
};
