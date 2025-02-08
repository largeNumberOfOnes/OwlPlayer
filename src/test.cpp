#include <chrono>
#include <iostream>
#include <thread>
#include "abstract.h"
#include "player/fileManager.h"


int main() {

    Abstract abs;
    FileManager manager{abs, "/home/dt/Documents"};

    while (true) {
        manager.reload();
        manager.draw(0, 0, abs.get_width(), abs.get_height());

        abs.update();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

