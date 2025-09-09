/**
 * The Player class is a graphical wrapper over had::Audio.
 */

#pragma once

#include "had/had.h"
#include "had/had_types.h"
#include <functional>



class Player {
    had::Audio audio;
    const had::Logger& log;
    had::Drawer& drawer;
    std::string DEFAULT_COMPOSITION_PATH = "******";
    std::string cur_composition_path = DEFAULT_COMPOSITION_PATH;

    std::string source_str = "---";

    struct Grid {
        had::Dem vol_x = 0;
        had::Dem vol_y = 0;
        had::Dem vol_w = 0;

        had::Dem tim_x = 0;
        had::Dem tim_y = 0;
        had::Dem tim_w = 0;
    } grid;
    had::Res calc_grid();

    had::Res draw_volume();
    had::Res draw_timeline();
    had::Res draw_composition_name();
    had::Res draw_source_str();

    bool is_comp_name_hidden = false;

    using CallOnPlayEnd = std::function<void(void)>;
    CallOnPlayEnd call_on_play_end = []() {};

    public:
        Player(had::Drawer& drawer, const had::Logger& log);
        Player(had::Drawer& drawer, CallOnPlayEnd call_on_play_end,
                                                const had::Logger& log);
        ~Player();

        had::Dem get_height();

        void set_on_play_end(CallOnPlayEnd call_on_play_end) {
            this->call_on_play_end = call_on_play_end;
        } // DEV [Should be replaced in source file]

        bool is_enougth_space();
        had::Res draw();
        had::Res resize();

        had::Res load_and_play(std::string_view path);
        had::Res play_or_stop();
        had::Res jump(had::seconds pos_rel);
        had::Res jump_rel(had::seconds pos_rel);

        void hide_comp_name();

        void set_source_str(std::string_view str);

        std::vector<
            std::vector<std::complex<float>>
        > get_cur_samples();
};
