#include <cstddef>
#include <cstdlib>
#include "had.h"
#include "ncurses.h"
#include <cmath>
#include <curses.h>
#include <fcntl.h>
#include <iterator>

#include <mutex>
#include <pipewire/pipewire.h>
#include <spa/param/audio/format-utils.h>
#include <stdio.h>

#include <sys/stat.h>
#include <thread>



namespace had::Colors {
    Color const black = COLOR_BLACK;
    Color const red = COLOR_RED;
    Color const green = COLOR_GREEN;
    Color const yellow = COLOR_YELLOW;
    Color const blue = COLOR_BLUE;
    Color const magenta = COLOR_MAGENTA;
    Color const cyan = COLOR_CYAN;
    Color const white = COLOR_WHITE;
}

res had::start_window() {
    initscr();
    start_color();
    noecho();
    // halfdelay(1);
    timeout(0);
    cbreak();
    curs_set(0);
    keypad(stdscr, true);
    return res::success;
}

res had::end_window() {
    endwin();
    return res::success;
}

had::key had::get_key() {
    int a = getch();
    switch (a) {
        case KEY_LEFT:  return key::arrow_left;
        case KEY_RIGHT: return key::arrow_rigth;
        case KEY_UP:    return key::arrow_up;
        case KEY_DOWN:  return key::arrow_down;
        case ' ':       return key::space;
        case 'q':       return key::q;
        default:        return key::nothing;
    }

}

void had::set_color() {
    // pass
}

void had::move() {
    // pass
}

res had::draw_slider(dem x, dem y, dem len, dem val) {
    std::string slider(len, '-');
    slider[val] = '0';
    mvprintw(y, x, "%s", slider.c_str());
    return res::success;
}

namespace had { // Drawing support
    res draw_text(dem x, dem y, char const* str){
        if (mvprintw(y, x, "%s", str) == OK)
            return res::success;
        else
            return res::error;
    }

    res draw_symbol(dem x, dem y, char ch) {
        if (mvaddch(y, x, ch) == OK)
            return res::success;
        else
            return res::error;
    }

    res draw_wide_symbol(dem x, dem y, wchar_t ch) {
        cchar_t cch;
        wchar_t ch_arr[2] = {ch, '\0'};
        setcchar(&cch, ch_arr, WA_NORMAL, 0, NULL);
        if (mvadd_wch(y, x, &cch) == OK)
            return res::success;
        else
            return res::error;
    }

    res update() {
        if (refresh() == OK)
            return res::success;
        else
            return res::error;
    }

    res cls() {
        if (clear() == OK)
            return res::success;
        else
            return res::error;
    }

    dem get_width() {
        int w = getmaxx(stdscr);
        return (w == ERR) ? 0 : w;
    }

    dem get_height() {
        int h = getmaxy(stdscr);
        return (h == ERR) ? 0 : h;
    }
}

namespace had { // Color support
    static int color_count = 10;
    static int pair_count = 1;

    res set_new_color(Color& col, int r, int g, int b) {
        if (color_count >= COLORS) {
            log_err("Too many colors");
            return res::error;
        }
        int ret = init_color(
            color_count, r*1000/256, g*1000/256, b*1000/256
        );
        if (ret == OK) {
            col = color_count++;
            return res::success;
        } else {
            col = 0;
            return res::error;
        }
    }

    res change_color(Color col, int r, int g, int b) {
        int ret = init_color(
            col, r*1000/256, g*1000/256, b*1000/256
        );
        if (ret == OK)
            return res::success;
        else
            return res::error;
    }

    res set_new_color_pair(ColorPair& pair, Color t, ColorPair b) {
        if (pair_count >= COLOR_PAIRS) {
            log_err("Too many color pairs");
            return res::error;
        }
        if (init_pair(pair_count, t, b) == OK) {
            pair = pair_count++;
            return res::success;
        } else {
            pair = 0;
            return res::error;
        }
    }

    res change_color_pair(ColorPair pair, Color t, ColorPair b) {
        if (init_pair(pair, t, b) == OK)
            return res::success;
        else
            return res::error;
    }

    res use_color_pair(ColorPair pair) {
        if (attron(COLOR_PAIR(pair)) == OK)
            return res::success;
        else
            return res::error;
    }

    res use_no_color_pair() {
        if (attroff(~0) == OK)
            return res::success;
        else
            return res::error;
    }
}
