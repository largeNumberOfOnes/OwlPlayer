#include "abstract.h"
#include "ncurses.h"
#include <curses.h>
#include <iterator>



Abstract::Abstract() {
    initscr();
    noecho();
    // halfdelay(1);
    timeout(0);
    cbreak();
    curs_set(0);
    keypad(stdscr, true);
}

Abstract::~Abstract() {
    endwin();
}

key Abstract::get_key() {
    int a = getch();
    switch (a) {
        case KEY_LEFT:  return key::arrow_left;
        case KEY_RIGHT: return key::arrow_rigth;
        case KEY_UP:    return key::arrow_up;
        case KEY_DOWN:  return key::arrow_down;
        default:        return key::nothing;
    }

}

void Abstract::set_color() {
    // pass
}

void Abstract::move() {
    // pass
}

stat Abstract::draw_slider(dem x, dem y, dem len, dem val) {
    std::string slider(len, '-');
    slider[val] = '0';
    mvprintw(y, x, "%s", slider.c_str());
    return stat::success;
}

stat Abstract::draw_text(dem x, dem y, char const* str){
    if (mvprintw(y, x, "%s", str) == OK)
        return stat::success;
    else
        return stat::error;
}

stat Abstract::update() {
    if (refresh() == OK)
        return stat::success;
    else
        return stat::error;
}

stat Abstract::cls() {
    if (clear() == OK)
        return stat::success;
    else
        return stat::error;
}

dem Abstract::get_width() {
    int w = 0, h = 0;
    getmaxyx(stdscr, h, w);
    return w;
}

dem Abstract::get_height() {
    int w = 0, h = 0;
    getmaxyx(stdscr, h, w);
    return h;
}

stat Abstract::load(std::string const& path) {
    // pass
    return stat::success;
}

stat Abstract::remove() {
    // pass
    return stat::success;
}

seconds Abstract::get_duration() {
    // pass
    return 7;
}

seconds Abstract::get_cur_time() {
    // pass
    return 7;
}

void Abstract::jump(int pos) {
    // pass
}
