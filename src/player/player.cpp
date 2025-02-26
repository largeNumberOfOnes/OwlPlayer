#include "player.h"

#include "../glob_types.h"
#include "../had.h"

#include <cstring>



static had::dem const vol_min_width = 12;
static had::dem const vol_max_width = 20;
static had::dem const tim_min_width = 26;
static had::dem const tim_half_width = 40;
static had::dem const vol_tim_pad = 3;
static had::dem const min_width = vol_min_width + tim_min_width +
                                                        vol_tim_pad;
static had::dem const min_height = 5;

struct Grid {
    had::dem vol_x = 0;
    had::dem vol_y = 0;
    had::dem vol_w = 0;

    had::dem tim_x = 0;
    had::dem tim_y = 0;
    had::dem tim_w = 0;
};

static res calc_grid(had::dem x, had::dem y, had::dem w, Grid& grid) {
    // From tim_min_width tim_w increases throw tim_half_width, while
    //      vol_w is constant.
    // When vol_w increases from vol_min_width throw vol_max_width. It is 
    //      vol_w maximum. tim_w is constant.
    // When only tim_w increases, vol_w is constant forever.

    if (w < min_width) {
        log_err("Error: cannot calculate vol_tim: width is too small");
        return res::error;
    }

    if (w < vol_min_width + vol_tim_pad + tim_half_width){
        grid.vol_w = vol_min_width;
        grid.tim_w = w - vol_min_width - vol_tim_pad;
    } else if (w < vol_max_width + vol_tim_pad + tim_half_width) {
        grid.vol_w = w - tim_half_width - vol_tim_pad;
        grid.tim_w = tim_half_width;
    } else {
        grid.vol_w = vol_max_width;
        grid.tim_w = w - vol_max_width - vol_tim_pad;
    }

    grid.vol_x = x;
    grid.tim_x = x + grid.vol_w + vol_tim_pad;
    grid.vol_y = y + 2;
    grid.tim_y = y + 2;

    return res::success;
}

static res draw_volume(had::dem x, had::dem y, had::dem w,
                                                    had::volume vol) {
    // vol     -----0--      77%
    // <--p1-->        <-p2>
    //         <---h-->
    // <-----------w----------->
    had::dem const p1 = 4;
    had::dem const p2 = 1;
    had::dem h = w - p1 - p2 - 4;

    if (w < vol_min_width) {
        log_err("Cannot draw volume: width is too small!");
        return res::error;
    }

    char str_vol[5] = {0};
    if (0 <= vol && vol < 1000) {
        sprintf(str_vol, "%3d%%", vol);
    } else {
        strcpy(str_vol, " inf");
    }


    had::dem slider_pos = h*vol/100;
    if (slider_pos >= h) slider_pos = h-1;

    if (false
        || had::draw_text(x, y, "vol ")
        || had::draw_slider(x + p1, y, h, slider_pos)
        || had::draw_text(x + p1 + h, y, " ")
        || had::draw_text(x + p1 + h + p2, y, str_vol)
    ) {
        log_err("Cannot draw volume");
        return res::error;
    }

    return res::success;
}

static std::string format_time(had::seconds time) {
    char str[6] = {0};
    sprintf(str, "%2d:%02d", time/60, time-(time/60));
    return std::string(str);
}

static res draw_timeline(had::dem x, had::dem y, had::dem w) {
    //  0:21   -----0--      3:21      <  &  >
    // <--p1-->        <-p2>     <-p3->
    //         <---h-->
    // <-----------w------------------------->
    had::dem const p1 = 5 + 1;
    had::dem const p2 = 1;
    had::dem const p3 = 4;
    had::dem h = w - p1 - p2 - p3 - 7 - 5;
    
    if (w < tim_min_width) {
        log_err("Error: cannot draw timeline: width is too small!");
        return res::error;
    }

    had::seconds cur_time = had::get_cur_time();
    had::seconds duration = had::get_duration();
    if (false
        || had::draw_text(x, y, format_time(cur_time).c_str())
        || had::draw_slider(x + p1, y, h, (h-1)*cur_time/duration)
        || had::draw_text(x + p1 + h + p2, y, format_time(duration).c_str())
        || had::draw_text(x + p1 + h + p2 + 5 + p3, y, "<  &  >")
    ) {
        log_err("Error: cannot draw timeline");
        return res::error;
    }

    return res::success;
}

res Player::draw(had::dem x, had::dem y, had::dem w) {

    Grid grid;
    if (calc_grid(x, y, w, grid)) {
        log_err("Not enought space to draw player");
        return res::error;
    }

    if (false
        || draw_volume(grid.vol_x, grid.vol_y, grid.vol_w, vol)
        || draw_timeline(grid.tim_x, grid.tim_y, grid.tim_w)
    ) {
        log_err("Cannot draw player");
        return res::error;
    }

    return res::success;
}

res stop() {
    return res::error;
}

res play() {
    return res::error;
}
