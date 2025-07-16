#include "errorBar.h"



ErrorBar::ErrorBar(had::Drawer& drawer, const Setup& setup,
                                                    const had::Logger& log)
    : drawer(drawer)
    , setup(setup)
    , log(log)
{}

void ErrorBar::set_error(std::string_view str) {
    error_str = str;
}

void ErrorBar::clear_error() {
    error_str = DEF_ERROR_STR;
}

had::Res ErrorBar::resize() {
    return had::Res::success;
}

had::Res ErrorBar::draw() {
    drawer.cls();
    if (error_str == DEF_ERROR_STR) {
        return had::Res::success;
    } else {
        if (false
            || drawer.set_color(setup.colors.error_str)
            || drawer.draw_text(0, 0, "Error: " + error_str)
            || drawer.set_color(setup.colors.def)
        ) {
            return had::Res::error;
        }
        return had::Res::success;
    }
}