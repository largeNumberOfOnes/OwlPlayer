#include "app.h"

#include "had/had.h"
#include "player.h"

#include <chrono>
#include <iostream>
#include <thread>



App::App(had::Interface& interface, Setup& setup, const had::Logger& log)
    : interface(interface)
    , setup(setup)
    , log(log)
    , player_drawer(interface, 0, 0, 0, 0, log)
    , player(player_drawer, log)
    , manager_drawer(interface, 0, 0, 0, 0, log)
    , manager(
        setup.get_default_file_dir(),
        [&](std::string path) { player.load_and_play(path); },
        manager_drawer, setup, log)
{
    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::draw;
        },
        [&](const Event& event) -> void {
            // manager.resize(manager_drawer.get_heigth());
            // manager.reload();
            manager.draw();
            player.draw();
        }
    );
    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::resize;
        },
        [&](const Event& event) -> void {
            // player_drawer.set(
            //     0, interface.get_height() - player.get_height(),
            //     interface.get_width(), player.get_height()
            // );
            // manager_drawer.set(
            //     0, 0,
            //     interface.get_width(),
            //     interface.get_height() - player.get_height()
            // );
            // manager.resize(manager_drawer.get_height());
            manager.resize();
            player.resize();
            // manager.reload();
        }
    );
    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::keypress
                && event.seq == had::KeySequence{had::Key::q};
        },
        [&](const Event& event) -> void {
            is_time_to_exit = true;
        }
    );
    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::keypress
                && event.seq == had::KeySequence{had::Key::arrow_up};
        },
        [&](const Event& event) -> void {
            manager.up();
        }
    );
    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::keypress
                && event.seq == had::KeySequence{had::Key::arrow_down};
        },
        [&](const Event& event) -> void {
            manager.down();
        }
    );
    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::keypress
                && event.seq == had::KeySequence{had::Key::j}.add_ctrl(); // DEV [enter]
        },
        [&](const Event& event) -> void {
            // manager_drawer.draw_text(7, 7, "go meth");
            manager.go();
        }
    );
    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::keypress
                && event.seq == had::KeySequence{had::Key::backspace};
        },
        [&](const Event& event) -> void {
            manager.back();
        }
    );
    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::keypress
                && event.seq == had::KeySequence{had::Key::space};
        },
        [&](const Event& event) -> void {
            player.play_or_stop();
        }
    );
}

App::Circle_res App::circle() {
    interface.set_color(setup.colors.def);
    player_drawer.set(
        0, interface.get_height() - player.get_height(),
        interface.get_width(), player.get_height()
    );
    manager_drawer.set(
        0, 0,
        interface.get_width(),
        interface.get_height() - player.get_height()
    );

    // interface.cls();

    had::KeySequence seq = interface.catch_key_seq();
    if (!seq.is_empty()) {
        event_queue.push_event(Event::create_keypress(seq));
    }
    if (interface.is_resized()) {
        event_queue.push_event(Event::create_resize());
    }
    event_queue.push_event(Event::create_draw());
    while (!event_queue.empty()) {
        event_queue.pop_event();
    }

    interface.update();

    if (is_time_to_exit) {
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
