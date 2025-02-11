#include "abstract.h"
#include "ncurses.h"
#include <curses.h>
#include <iterator>

stat had::start_window() {
    initscr();
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

void had::jump(had::seconds pos) {
    // pass
}
