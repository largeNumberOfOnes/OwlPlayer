#include "queuePanel.h"

#include "had/had.h"

#include <queue>
#include <span>
#include <string_view>
#include <optional>
#include <string>



QueuePanel::QueuePanel(had::Drawer& drawer, const had::Logger& log)
    : drawer(drawer), log(log)
{}

had::Res QueuePanel::draw() {
    drawer.cls();
    if (list.empty()) {
        return drawer.draw_text(0, 0, "Queue is empty");
    }

    auto it = list.cbegin();
    for (int q = 0; q < drawer.get_height() && it != list.cend(); ++q,
                                                                    ++it) {
        drawer.draw_text(0, q, *it);
    }

    return had::Res::success;
}

had::Res QueuePanel::resize() {
    return had::Res::success;
}

void QueuePanel::add(std::string_view file) {
    list.push_back(std::string(file));
}

std::optional<std::string> QueuePanel::pop() {
    if (!list.empty()) {
        std::string ret = list.front();
        list.pop_front();
        return ret;
    } else {
        return std::nullopt;
    }
}
