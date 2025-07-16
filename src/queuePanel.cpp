#include "queuePanel.h"



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

std::string_view QueuePanel::pop() {
    return "NONE"; // DEV
}
