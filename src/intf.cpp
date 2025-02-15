// #include "abstract.h"
#include "intf.h"
#include "had.h"
#include <curses.h>
// #include <cstdio>
// #include <iostream>
// #include <cstring>



// Interface::Interface(Abstract& abs) : abs(abs) {
//     // pass
// }

// stat Interface::draw_topPannel(dem w, dem h) {

//     if (!top_data_is_set) {
//         abs.draw_text(0, 0, "None");
//         return stat::success;
//     }

//     // if (true
//     //     && top_main_dir_name != nullptr
//     //     && top_files != nullptr
//     //     && top_dirs != nullptr
//     //     && top_search_pat != nullptr
//     // ) {
//     //     std::cerr << "Error: cannot draw top: bad data" << std::endl;
//     //     return stat::error;
//     // }

//     if (w < top_min_width) {
//         std::cerr << "Error: cannot draw top: width is too small"
//                                                             << std::endl;
//         return stat::error;
//     }

//     // abs.draw_text(0, 0, top_main_dir_name);
//     // int pos = 1;
//     // for (int q = 0; q < top_dirs_count; ++q) {
//     //     abs.draw_text(2, q+1, top_dirs[q]);
//     // }
//     // for (; q < top_dirs_count; ++q) {
//     //     abs.draw_text(3, q+1, top_dirs[q]);
//     // }

//     return stat::success;
// }

// stat Interface::draw_volume(dem x, dem y, dem w) {
//     // vol     -----0--      77%
//     // <--p1-->        <-p2>
//     //         <---h-->
//     // <-----------w----------->
//     dem const p1 = 4;
//     dem const p2 = 1;
//     dem h = w - p1 - p2 - 4;

//     if (w < vol_min_width) {
//         std::cerr << "Error: cannot draw volume: width is too small!"
//                                                             << std::endl;
//         return stat::error;
//     }

//     char str_vol[5] = {0};
//     if (0 <= vol && vol < 1000) {
//         sprintf(str_vol, "%3d%%", vol);
//     } else {
//         strcpy(str_vol, " inf");
//     }

//     dem slider_pos = h*vol/100;
//     if (slider_pos >= h) slider_pos = h-1;

//     if (false
//         || abs.draw_text(x, y, "vol ")
//         || abs.draw_slider(x + p1, y, h, slider_pos)
//         || abs.draw_text(x + p1 + h, y, " ")
//         || abs.draw_text(x + p1 + h + p2, y, str_vol)
//     ) {
//         std::cerr << "Error: cannot draw volume" << std::endl;
//         return stat::error;
//     }

//     return stat::success;
// }

// static std::string format_time(seconds time) {
//     char str[6] = {0};
//     sprintf(str, "%2d:%02d", time/60, time-(time/60));
//     return std::string(str);
// }

// stat Interface::draw_timeline(dem x, dem y, dem w) {
//     //  0:21   -----0--      3:21      <  &  >
//     // <--p1-->        <-p2>     <-p3->
//     //         <---h-->
//     // <-----------w------------------------->
//     dem const p1 = 5 + 1;
//     dem const p2 = 1;
//     dem const p3 = 4;
//     dem h = w - p1 - p2 - p3 - 7 - 5;
    
//     if (w < tim_min_width) {
//         std::cerr << "Error: cannot draw timeline: width is too small!"
//                                                             << std::endl;
//         return stat::error;
//     }

//     seconds cur_time = abs.get_cur_time();
//     seconds duration = abs.get_duration();
//     if (false
//         || abs.draw_text(x, y, format_time(cur_time))
//         || abs.draw_slider(x + p1, y, h, (h-1)*cur_time/duration)
//         || abs.draw_text(x + p1 + h + p2, y, format_time(duration))
//         || abs.draw_text(x + p1 + h + p2 + 5 + p3, y, "<  &  >")
//     ) {
//         std::cerr << "Error: cannot draw timeline" << std::endl;
//         return stat::error;
//     }

//     return stat::success;
// }

// stat Interface::calc_vol_tim(dem w, dem h) {
//     // From tim_min_width tim_w increases throw tim_half_width, while
//     //      vol_w is constant.
//     // When vol_w increases from vol_min_width throw vol_max_width. It is 
//     //      vol_w maximum. tim_w is constant.
//     // When only tim_w increases, vol_w is constant forever.

//     if (w < min_width) {
//         std::cerr << "Error: cannot calculate vol_tim: width is too small"
//                                                             << std::endl;
//         return stat::error;
//     }

//     if (w < vol_min_width + vol_tim_pad + tim_half_width){
//         imet.vol_w = vol_min_width;
//         imet.tim_w = w - vol_min_width - vol_tim_pad;
//     } else if (w < vol_max_width + vol_tim_pad + tim_half_width) {
//         imet.vol_w = w - tim_half_width - vol_tim_pad;
//         imet.tim_w = tim_half_width;
//     } else {
//         imet.vol_w = vol_max_width;
//         imet.tim_w = w - vol_max_width - vol_tim_pad;
//     }

//     imet.tim_x = imet.vol_w + vol_tim_pad;
//     imet.vol_y = h - 2;
//     imet.tim_y = h - 2;

//     return stat::success;
// }

// stat Interface::calc_top(dem w, dem h) {
//     if (h < draw_top_h_trig) {
//         imet.top_draw = false;
//         return stat::success;
//     }

//     dem heig = h - min_height;

//     return stat::success;
// }

// stat Interface::calc_int_metrics(dem w, dem h) {
    
//     if (false
//         || calc_vol_tim(w, h)
//     ) {
//         std::cerr << "Error: cannot calculate metrics" << std::endl;
//         return stat::error;
//     }

//     return stat::success;
// }

// stat Interface::draw() {
//     dem w = abs.get_width();
//     dem h = abs.get_height();
//     if (w != old_w || h != old_h) {
//         old_w = w;
//         old_h = h;

//         abs.cls();

//         calc_int_metrics(w, h);
//     }

//     if (w < min_width || h < min_height) {
//         abs.draw_text(0, 0, "No spase");
//         return stat::success;
//     }
    
//     if (false
//         || draw_topPannel(w, h)
//         // || draw_name()
//         // || draw_source()
//         || draw_volume(imet.vol_x, imet.vol_y, imet.vol_w)
//         || draw_timeline(imet.tim_x, imet.tim_y, imet.tim_w)
//     ) {
//         std::cerr << "Error: cannot draw interface" << std::endl;
//         return stat::error;
//     }

//     abs.update();

//     return stat::success;
// }

// volume Interface::get_vol() {
//     return vol;
// }

// void Interface::set_vol(volume vol) {
//     this->vol = vol;
// }

// volume Interface::get_tim() {
//     return tim;
// }

// void Interface::set_tim(volume vol) {
//     this->vol = tim;
// }


namespace intf {

    static had::Color color_select_backgraund = had::Colors::black;

    static had::ColorPair pair_dir = had::default_color_pair;
    static had::ColorPair pair_dir_selected = had::default_color_pair;
    static had::ColorPair pair_file = had::default_color_pair;
    static had::ColorPair pair_file_selected = had::default_color_pair;

    stat init_colors() {
        if (false
            || had::set_new_color(color_select_backgraund, 60, 60, 60)
            || had::set_new_color_pair(pair_dir, had::Colors::blue, had::Colors::black)
            || had::set_new_color_pair(pair_dir_selected, had::Colors::blue, color_select_backgraund)
            || had::set_new_color_pair(pair_file, had::Colors::white, had::Colors::black)
            || had::set_new_color_pair(pair_file_selected, had::Colors::white, color_select_backgraund)
        ) return stat::error;
        return stat::success;
    }

    stat draw_list_symbol(had::dem x, had::dem y) {
        return had::draw_wide_symbol(x, y, L'├');
    }

    stat draw_list_end_symbol(had::dem x, had::dem y) {
        return had::draw_wide_symbol(x, y, L'└');
    }

    stat setcol_dir() {
        return had::use_color_pair(pair_dir);
    }
    stat setcol_file() {
        return had::use_color_pair(pair_file);    
    }
    stat setcol_dir_selected() {
        return had::use_color_pair(pair_dir_selected);
    }
    stat setcol_file_selected() {
        return had::use_color_pair(pair_file_selected);
    }
    stat setcol_default() {
        // return had::use_color_pair(had::default_color_pair);
        return had::use_no_color_pair();
    }

}