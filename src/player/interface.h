#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "abstract.h"



class Interface {
    private:
        std::string cur_name = "Unknown";
        Abstract& abs;
        volume vol = 0;
        seconds tim = 0;

        static dem const min_h_to_draw_topPannel = 4;
        static dem const vol_min_width = 12;
        static dem const tim_min_width = 26;
        static dem const vol_tim_pad = 3;
        static dem const min_width = vol_min_width + tim_min_width +
                                                            vol_tim_pad;
        static dem const draw_top_h_trig = 10;
        static dem const min_height = 5;
        static dem const tim_half_width = 40;
        static dem const vol_max_width = 20;
        static dem const top_min_width = 10;

        bool top_data_is_set = false;
        // std::string const& top_main_dir_name;
        // std::vector<std::string> const& top_files;
        // int top_files_count = 0;
        // std::vector<std::string> const& top_dirs;
        // int top_dirs_count = 0;
        // std::string const& top_search_pat = nullptr;

        struct Int_metrics {
            dem vol_w = 0;
            dem vol_x = 0;
            dem vol_y = 0;

            dem tim_w = 0;
            dem tim_x = 0;
            dem tim_y = 0;

            dem top_h = 0;
            bool top_draw = false;
        };
        Int_metrics imet;
        stat calc_vol_tim(dem w, dem h);
        stat calc_top(dem w, dem h);
        stat calc_int_metrics(dem w, dem h);

        stat draw_topPannel(dem w, dem h);

        stat draw_volume(dem x, dem y, dem w);
        stat draw_timeline(dem x, dem y, dem w);

        dem old_w = 0;
        dem old_h = 0;

    public:
        Interface(Abstract& abs);
        stat draw();

        void set_top_data();

        volume get_vol();
        void set_vol(volume vol);

        volume get_tim();
        void set_tim(seconds tim);
};
