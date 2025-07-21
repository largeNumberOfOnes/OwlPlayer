#include "queuePanel.h"

#include <string_view>
#include <optional>
#include <string>



QueuePanel::QueuePanel(had::Drawer& drawer, const had::Logger& log)
    : drawer(drawer), log(log)
{}

had::Res QueuePanel::draw() {
    drawer.draw_text(0, 0, "queue");
    return had::Res::success;
}

had::Res QueuePanel::resize() {
    return had::Res::success;
}

void QueuePanel::add(std::string_view file) {
    queue.push(std::string(file));
}

void QueuePanel::add_from_vector(const std::vector<std::string>& vec) {
    for (const auto& it : vec) {
        queue.push(it);
    }
}

std::optional<std::string> QueuePanel::pop() {
    if (!queue.empty()) {
        std::string ret = queue.back();
        queue.pop();
        return ret;
    } else {
        return std::nullopt;
    }
}
