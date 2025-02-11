#include <chrono>
#include <iostream>
#include <thread>
#include "abstract.h"
#include "player/fileManager.h"


int main() {

    FileManager manager{"/home/dt/Documents"};
    had::start_window();
    while (true) {

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
        manager.draw(0, 0, had::get_width(), had::get_height());

        had::update();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    had::end_window();

    return 0;
}

