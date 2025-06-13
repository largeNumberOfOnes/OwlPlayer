#include "had_interface.h"
#include "had_logger.h"

#include <ncurses.h>

int main() {

    had::Interface interface{had::blacklogger};
    had::Drawer drawer = interface.produce_drawer();

    had::Color col_red;
    had::Color col_blue_pink;
    had::Color col_def;
    interface.create_text_color(col_red, 223, 56, 12);
    interface.create_color(col_blue_pink, 34, 12, 200, 200, 100, 150);
    interface.get_default_color(col_def);
    drawer.set_color(col_def);
    drawer.draw_text(0, 0, "(default, default) hello!");
    drawer.set_color(col_red);
    drawer.draw_text(0, 1, "(red, default) hello!");
    drawer.set_color(col_blue_pink);
    drawer.draw_text(0, 2, "(blue, pink) hello!");
    drawer.set_color(col_def);
    drawer.draw_text(0, 3, "(default, default) hello!");

    // int pair = 30;
    // for (int q = 0; q < COLORS; ++q) {
    //     init_pair(pair++, q, COLOR_BLACK);
    //     // drawer.set_color(pair);
    //     attron(COLOR_PAIR(pair));
    //     drawer.draw_text((q%10) * 7, q/10, "hello!");
    // }

    interface.update();

    while (true) {}

    return 0;
}
