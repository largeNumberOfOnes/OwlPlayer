/**
 * This class represents a graphical player. It contains the main loop
 *    of the program.
 */


#pragma once

#include "inputWrapper.h"
#include "fileManager.h"
#include "switchPanel.h"
#include "queuePanel.h"
#include "eventQueue.h"
#include "shuffler.h"
#include "errorBar.h"
#include "had/had.h"
#include "spectre.h"
#include "player.h"
#include "setup.h"

#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>



class App {
    had::Volume jump_val = 5;

    using Action = std::string;
    using Actions = std::unordered_map<Action, std::function<void(void)>>;
    Actions actions {
        {"manager_go"  , [this]() {
                switch_panel.call_wrapper(
                    manager_id,
                    [this]() {
                        if (manager.go()) {
                            log.log_err("Cannot exec manager.go()");
                            error_bar.set_error(
                                "Not playable file or directory");
                        }
                        shuffler.set_sequential(
                            manager.get_dirs_files()
                        );
                    }
                );
            }},
        {"manager_back", [this]() {
                switch_panel.call_wrapper(
                    manager_id,
                    [this]() { manager.back(); }
                );
            }},
        {"manager_up"  , [this]() {
                switch_panel.call_wrapper(
                    manager_id,
                    [this]() { manager.up(); }
                );
            }},
        {"manager_down", [this]() {
                switch_panel.call_wrapper(
                    manager_id,
                    [this]() { manager.down(); }
                );
            }},
        {"manager_search", [this]() {
                switch_panel.call_wrapper(
                    manager_id,
                    [this]() {
                        manager.search_start();
                        input_wrapepr.capture(
                            [&manager = manager](std::string_view str, int curs_pos) // DEV [Refactoring is needed]
                                { manager.search_set_string(str, curs_pos); },
                            [&manager = manager](std::string&& str) {
                                manager.search_stop();
                            }
                        );
                    }
                );
            }},
        {"play_stop",      [this]() { player.play_or_stop(); }},
        {"play_inc",       [this]() { player.jump_rel(jump_val); }},
        {"play_dec",       [this]() { player.jump_rel(-jump_val); }},
        {"play_inc_vol",   [this]() {  }},
        {"play_dec_vol",   [this]() {  }},
        {"play_hide_name", [this]() { player.hide_comp_name(); }},
        {"queue_add", [this]() {
                std::optional<std::string> comp =
                                            manager.get_selected_comp();
                if (comp.has_value()) {
                    queue.add(comp.value());
                } else {
                    std::string_view err =
                        "Cannot add not playable composition into queue";
                    log.log_err(err);
                    error_bar.set_error(err);
                }
            }},
        {"shuf_rot_mode", [this]() {
                shuffler.rotate_modes_cicle();
                player.set_source_str(shuffler.get_mode_str());
            }},
        {"glob_quit",      [this]() { is_it_time_to_exit = true; }},
        {"panel_next",     [this]() { 
            log.log_info("panel_next()");
            switch_panel.inc(); }},
    };

    had::Interface& interface;
    Setup& setup;
    const had::Logger& log;
    InputWrapper input_wrapepr;
    had::Drawer error_drawer;
    had::Drawer player_drawer;
    had::Drawer switch_panel_drawer;

    EventQueue event_queue;
    ErrorBar error_bar;
    Player player;
    FileManager manager;
    Spectre spectre;
    QueuePanel queue;

    SwitchPanel switch_panel;
    SwitchPanel::ComponentId manager_id;
    SwitchPanel::ComponentId spectre_id;

    Shuffler shuffler{manager, queue, log};

    enum class Circle_res {
        success,
        exit,
    };
    bool is_it_time_to_exit = false;
    Circle_res circle();

    had::Res process_keypress();

    public:
        App(had::Interface& interface, Setup& setup,
                                                const had::Logger& log);

        void run();
};
