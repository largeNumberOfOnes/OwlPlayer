/**
 * pass
 */

#pragma once

#include "had/had.h"

#include <optional>
#include <string_view>
#include <span>
#include <string>
#include <list>
#include <vector>



class QueuePanel {
    std::list<std::string> list;

    had::Drawer& drawer;
    const had::Logger& log;
    
    public:
        QueuePanel(had::Drawer& drawer, const had::Logger& log);
        void add(std::string_view file);
        std::optional<std::string> pop();
        
        had::Res draw();
        had::Res resize();
};
