/**
 * Graphical error bar.
 */

#pragma once

#include "had/had.h"
#include "setup.h"



class ErrorBar {
    had::Drawer& drawer;
    const Setup& setup;
    const had::Logger& log;

    const std::string DEF_ERROR_STR = "test error";
    std::string error_str = "test error";

    public:
        ErrorBar(had::Drawer& drawer, const Setup& setup,
                                                const had::Logger& log);

        void set_error(std::string_view str);
        void clear_error();

        had::Res resize();
        had::Res draw();
};
