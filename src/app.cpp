// clangd: no-unused-includes
#include "app.h"
#include "had/had.h"
#include "queuePanel.h"

#include <optional>
#include <random>
#include <string_view>
#include <iostream>
#include <chrono>
#include <string>
#include <thread>



App::App(had::Interface& interface, Setup& setup, const had::Logger& log)
    : interface(interface)
    , setup(setup)
    , log(log)
    , error_drawer(interface, log)
    , player_drawer(interface, log)
    , switch_panel_drawer(interface, log)
    , error_bar(error_drawer, setup, log)
    , player(
            player_drawer,
            [this, &log]() {
                manager.release();
                std::optional<std::string> file =
                                        shuffler.get_next_composition();
                if (file.has_value()) {
                    manager.set_playing_file(file.value());
                    log.log_info("comp " + file.value());
                } else {
                    log.log_info("no comp");
                }
            },
            log
        )
    , manager(
        setup.get_default_file_dir(),
        [this](std::string_view path) {
            player.load_and_play(path);
        },
        switch_panel_drawer, setup, log)
    , spectre(
        switch_panel_drawer,
        [this](Spectre::DataArray& data) {
            player.get_cur_samples(data);
        },
        log)
    , queue(switch_panel_drawer, log)
{
    manager_id = switch_panel.add_component(
        [this]() -> had::Res { return manager.draw(); },
        [this]() -> had::Res { return manager.resize(); }
    );
    spectre_id = switch_panel.add_component(
        [this]() -> had::Res { return spectre.draw(); },
        [this]() -> had::Res { return spectre.resize(); }
    );
    spectre_id = switch_panel.add_component(
        [this]() -> had::Res { return queue.draw(); },
        [this]() -> had::Res { return queue.resize(); }
    );

    shuffler.set_mode(setup.get_default_shuffler_mode());
    player.set_source_str(shuffler.get_mode_str());

    event_queue.add_observer(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::draw;
        },
        [&](const Event& event) -> void {
            error_bar.draw();
            player.draw();
            switch_panel.draw();
        }
    );
    event_queue.add_observer(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::resize;
        },
        [&](const Event& event) -> void {
            error_bar.resize();
            switch_panel.resize();
            player.resize();
        }
    );

    for (const auto& it : setup.get_key_bindings()) {
        if (actions.contains(it.first)) {
            event_queue.add_observer(
                [=, &setup](const Event& event) -> bool {
                    return event.type == Event::EventType::keypress
                        && event.seq == it.second;
                },
                [&](const Event& event) -> void {
                    actions[it.first]();
                }
            );
        } else {
            log.log_warn("Setup file contains keybinding for "
                                                    "nonexistent action");
        }
    }
}

had::Res App::process_keypress() {
    had::KeySequence seq = interface.catch_key_seq();
    if (!seq.is_empty()) {
        event_queue.push_event(Event::create_keypress(seq));
    }

    return had::Res::success;
}

App::Circle_res App::circle() {
    interface.set_color(setup.colors.def);

    error_drawer.set(
        0, interface.get_height() - 1,
        interface.get_width(), 1
    );
    player_drawer.set(
        0, interface.get_height() - player.get_height() - 1,
        interface.get_width(), player.get_height()
    );
    switch_panel_drawer.set(
        0, 0,
        interface.get_width(),
        interface.get_height() - player.get_height() - 1
    );

    process_keypress();
    if (interface.is_resized()) {
        event_queue.push_event(Event::create_resize());
    }
    event_queue.push_event(Event::create_draw());

    event_queue.unwrap();
    interface.update();

    if (is_it_time_to_exit) {
        return Circle_res::exit;
    }
    return Circle_res::success;
}

void App::run() {

    while (true) {
        auto time_start = std::chrono::steady_clock::now();
        auto time_period = std::chrono::milliseconds(1000/setup.get_fps());
        Circle_res res = circle();
        if (res == Circle_res::exit) {
            break;
        }
        std::this_thread::sleep_until(time_start + time_period);
    }

}
