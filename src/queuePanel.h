/**
 * pass
 */

#pragma once

#include "had/had.h"

#include <optional>
#include <string_view>
#include <string>
#include <queue>
#include <vector>



class QueuePanel {
    std::queue<std::string> queue;
    had::Drawer& drawer;
    const had::Logger& log;
    public:
        QueuePanel(had::Drawer& drawer, const had::Logger& log);
        void add(std::string_view file);
        void add_from_vector(const std::vector<std::string>& vec);
        std::optional<std::string> pop();
        
        had::Res draw();
        had::Res resize();
};
