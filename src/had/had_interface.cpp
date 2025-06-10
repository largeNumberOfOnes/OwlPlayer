



#include "had_interface.h"


namespace had {
    Interface::Interface() {
        initscr();
        start_color();
        noecho();
        // halfdelay(1);
        timeout(0);
        cbreak();
        curs_set(0);
        keypad(stdscr, true);
    }

    Interface::~Interface() {
        endwin();
    }
}