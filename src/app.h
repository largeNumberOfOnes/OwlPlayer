/**
 * This class represents a graphical player. It contains the main loop
 *    of the program.
 */


#pragma once

#include "had/had.h"

#include "fileManager.h"
#include "eventQueue.h"
#include "player.h"
#include "setup.h"

#include <unordered_map>

class App {
    std::unordered_map<std::string, std::function<void(void)>> actions {
        {"manager_go"  , [&]() { manager.go(); }},
        {"manager_up"  , [&]() { manager.up(); }},
        {"manager_down", [&]() { manager.down(); }},
        {"play_stop", [&]() {  }},
        {"inc", [&]() {  }},
        {"dec", [&]() {  }},
        {"inc_vol", [&]() {  }},
        {"dec_vol", [&]() {  }},
    };

    const had::Logger& log;
    Setup& setup;
    had::Interface interface;

    EventQueue event_queue;

    had::Drawer player_drawer;
    Player player;

    had::Drawer manager_drawer;
    FileManager manager;

    enum class Circle_res {
        success,
        exit,
    };
    bool is_time_to_exit = false;
    Circle_res circle();

    public:
        App(had::Interface& interface, Setup& setup,
                                                const had::Logger& log);

        void run();
};
