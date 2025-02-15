#include "abstract.h"
#include "player.h"
#include "interface.h"
#include "manager.h"

#include <chrono>
#include <iostream>
#include <thread>

static const std::chrono::microseconds frame_duration(50);
// static const std::chrono::milliseconds frame_duration(1000);

int main(int argc, const char *const *argv) {

    Abstract abs;

    Player player{};
    Interface interface{abs};
    Manager manager{abs, player, interface};

    while (true) {
        auto start_time = std::chrono::system_clock::now();
        switch (manager.work()) {
            case manager_ret::success:
                std::this_thread::sleep_until(start_time + frame_duration);
                continue;
            case manager_ret::error:
                std::cerr << "Error: in manager.work()" << std::endl;
                break;
            case manager_ret::out:
                break;
        }
        break;
    }

}
