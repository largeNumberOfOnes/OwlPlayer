


#pragma once

#include "eventQueue.hpp"

class App {

    EventQueue event_queue;

    enum class Circle_res {
        success,
        exit,
    };
    Circle_res circle();

    public:
        App();

        void run();
};
