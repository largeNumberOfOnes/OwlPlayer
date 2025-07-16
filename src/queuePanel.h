/**
 * pass
 */

#pragma once

#include "had/had.h"

#include <string_view>
#include <string>
#include <queue>



class QueuePanel {
    std::queue<std::string> queue;
    had::Drawer& drawer;
    const had::Logger& log;
    public:
        QueuePanel(had::Drawer& drawer, const had::Logger& log);
        void add(std::string_view file);
        std::string_view pop();
        
        had::Res draw();
        had::Res resize();
};
