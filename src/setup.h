


#pragma once

#include "had/had.h"
#include "had/had_keys.h"

#include <string>
// #include <unordered_map>
#include <vector>



class Setup {
    public:
        using Action = std::string;
        using KeyBinding = std::pair<Action, had::KeySequence>;
        using BindingsList = std::vector<KeyBinding>;

    private:
        std::string default_file_dir = "/home/dt/Desktop/mus";

        int fps = 60; // DEV [This may seem like a lot, but it's only
                      // DEV      needed to properly capture keystrokes,
                      // DEV      so it's worth thinning out the rest.
                      // DEV      (previous value is 20) ]

        BindingsList key_bindings = {
            {"manager_go",     had::KeySequence{had::Key::j}.add_ctrl()}, // DEV [enter]
            {"manager_back",   had::KeySequence{had::Key::backspace}},
            {"manager_up",     had::KeySequence{had::Key::arrow_up}},
            {"manager_down",   had::KeySequence{had::Key::arrow_down}},
            {"play_stop",      had::KeySequence{had::Key::space}},
            {"play_inc",       had::KeySequence{had::Key::arrow_rigth}},
            {"play_dec",       had::KeySequence{had::Key::arrow_left}},
            {"glob_quit",      had::KeySequence{had::Key::q}},
            {"panel_next",     had::KeySequence{had::Key::s}},
            {"play_hide_name", had::KeySequence{had::Key::h}},
        };

    public:
        had::Res read_config(std::string path);
        struct Colors {
            had::Color def;
            had::Color dir;
            had::Color file;
            had::Color dir_selected;
            had::Color file_selected;
        } colors;

        int get_fps() const;
        const BindingsList& get_key_bindings() const;

        std::string get_default_file_dir();

};
