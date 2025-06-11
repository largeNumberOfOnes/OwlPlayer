/**
 * The Player class is a graphical wrapper over had::Audio.
 */

#pragma once

#include "had/had.h"



class Player {
    had::Audio audio;
    const had::Logger& log;
    had::Drawer& drawer;

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

    public:
        Player(had::Drawer& drawer, const had::Logger& log);

        had::Dem get_height();

        bool is_enougth_space();
        had::Res draw();

        had::Res stop();
        had::Res play();
};
