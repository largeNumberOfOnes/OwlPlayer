/**
 * This class represents a graphical player. It contains the main loop
 *    of the program.
 */


#pragma once

#include "had/had.h"

#include "eventQueue.hpp"
#include "player.h"

class App {

    const had::Logger& log;
    had::Interface interface;

    EventQueue event_queue;

    had::Drawer player_drawer;
    Player player;

    enum class Circle_res {
        success,
        exit,
    };
    Circle_res circle();

    public:
        App(const had::Logger& log);

        void run();
};
