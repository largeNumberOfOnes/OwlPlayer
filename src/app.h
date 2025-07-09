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

    had::Volume jump_val = 5;

    using Action = std::string;
    using Actions = std::unordered_map<Action, std::function<void(void)>>;
    Actions actions {
        {"manager_go"  , [this]() { manager.go(); }},
        {"manager_up"  , [this]() { manager.up(); }},
        {"manager_down", [this]() { manager.down(); }},
        {"play_stop",    [this]() { player.play_or_stop(); }},
        {"play_inc",     [this]() { player.jump_rel(jump_val); }},
        {"play_dec",     [this]() { player.jump_rel(-jump_val); }},
        {"play_inc_vol", [this]() {  }},
        {"play_dec_vol", [this]() {  }},
        {"glob_quit",    [this]() { is_time_to_exit = true; }},
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
