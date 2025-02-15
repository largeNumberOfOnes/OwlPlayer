#include "../src/had.h"

int test1() {
    had::start_window();
    had::end_window();
    return 0;
}

int test2() { // Test color
    had::start_window();

    had::Color col;
    had::set_new_color(col, 100, 0, 200);
    had::ColorPair pair;
    had::set_new_color_pair(pair, col, had::Colors::black);
    had::use_color_pair(pair);
    had::draw_text(0, 0, "Press q to continue");
    had::update();
    while (had::get_key() != had::key::q) {}

    had::end_window();
    return 0;
}

int main() {

    test1();
    test2();

    return 0;
}
