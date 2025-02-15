#include "had.h"
#include "ncurses.h"
#include <cmath>
#include <curses.h>
#include <iterator>

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

stat had::start_window() {
    initscr();
    start_color();
    noecho();
    // halfdelay(1);
    timeout(0);
    cbreak();
    curs_set(0);
    keypad(stdscr, true);
    return stat::success;
}

stat had::end_window() {
    endwin();
    return stat::success;
}

had::key had::get_key() {
    int a = getch();
    switch (a) {
        case KEY_LEFT:  return key::arrow_left;
        case KEY_RIGHT: return key::arrow_rigth;
        case KEY_UP:    return key::arrow_up;
        case KEY_DOWN:  return key::arrow_down;
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

stat had::draw_slider(dem x, dem y, dem len, dem val) {
    std::string slider(len, '-');
    slider[val] = '0';
    mvprintw(y, x, "%s", slider.c_str());
    return stat::success;
}

stat had::draw_text(dem x, dem y, char const* str){
    if (mvprintw(y, x, "%s", str) == OK)
        return stat::success;
    else
        return stat::error;
}

stat had::draw_symbol(dem x, dem y, char ch) {
    if (mvaddch(y, x, ch) == OK)
        return stat::success;
    else
        return stat::error;
}

stat had::draw_wide_symbol(dem x, dem y, wchar_t ch) {
    cchar_t cch = {0, ch};
    if (mvadd_wch(y, x, &cch) == OK)
        return stat::success;
    else
        return stat::error;
}

stat had::update() {
    if (refresh() == OK)
        return stat::success;
    else
        return stat::error;
}

stat had::cls() {
    if (clear() == OK)
        return stat::success;
    else
        return stat::error;
}

had::dem had::get_width() {
    int w = 0, h = 0;
    getmaxyx(stdscr, h, w);
    return w;
}

had::dem had::get_height() {
    int w = 0, h = 0;
    getmaxyx(stdscr, h, w);
    return h;
}

namespace had { // Color support
    static unsigned int color_count = 10;
    static unsigned int pair_count = 1;

    stat set_new_color(Color& col, int r, int g, int b) {
        if (color_count >= COLORS) {
            log_err("Too many colors");
            return stat::error;
        }
        int ret = init_color(
            color_count, r*1000/256, g*1000/256, b*1000/256
        );
        if (ret == OK) {
            col = color_count++;
            return stat::success;
        } else {
            col = 0;
            return stat::error;
        }
    }

    stat change_color(Color col, int r, int g, int b) {
        int ret = init_color(
            col, r*1000/256, g*1000/256, b*1000/256
        );
        if (ret == OK)
            return stat::success;
        else
            return stat::error;
    }

    stat set_new_color_pair(ColorPair& pair, Color t, ColorPair b) {
        if (pair_count >= COLOR_PAIRS) {
            log_err("Too many color pairs");
            return stat::error;
        }
        if (init_pair(pair_count, t, b) == OK) {
            pair = pair_count++;
            return stat::success;
        } else {
            pair = 0;
            return stat::error;
        }
    }

    stat change_color_pair(ColorPair pair, Color t, ColorPair b) {
        if (init_pair(pair, t, b) == OK)
            return stat::success;
        else
            return stat::error;
    }

    stat use_color_pair(ColorPair pair) {
        if (attron(COLOR_PAIR(pair)) == OK)
            return stat::success;
        else
            return stat::error;
    }

    stat use_no_color_pair() {
        if (attroff(~0) == OK)
            return stat::success;
        else
            return stat::error;
    }
}

stat had::load(char const* path) {
    // pass
    return stat::success;
}

stat had::remove() {
    // pass
    return stat::success;
}

had::seconds had::get_duration() {
    // pass
    return 7;
}

had::seconds had::get_cur_time() {
    // pass
    return 7;
}

stat had::jump(had::seconds pos) {
    // pass
    return stat::success;
}
