#include <chrono>
#include <iostream>
#include <thread>

#include "../src/had.h"
#include "../src/intf.h"
#include "../src/player/fileManager.h"


int main() {

    FileManager manager{"/home/dt/Documents"};
    had::start_window();
    intf::init_colors();

    while (true) {

        int w = had::get_width();
        int h = had::get_height();
        int p = 3;
        char ch = '#';
        for (int x = p; x < w-p; ++x) {
            had::draw_symbol(x, p, ch);
            had::draw_symbol(x, h-p-1, ch);
        }
        for (int y = p; y < h-p; ++y) {
            had::draw_symbol(p, y, ch);
            had::draw_symbol(w-p-1, y, ch);
        }

        switch (had::get_key()) {
            case had::key::arrow_up:
                manager.up();
                break;
            case had::key::arrow_down:
                manager.down();
                break;
            default:
                break;
        }
        manager.reload();
        // manager.resize(had::get_height()-2);
        manager.draw(p+1, p+1, had::get_width()-2*p-2, had::get_height()-2*p-2);

        had::update();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    had::end_window();

    return 0;
}

