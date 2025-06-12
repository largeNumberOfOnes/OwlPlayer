#include "app.h"

#include "had/had.h"
#include "had/had_interface.h"
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
    , manager(setup.get_default_file_dir(), manager_drawer, setup, log)
{
    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::draw;
        }, 
        [&](const Event& event) -> void {
            player.draw();
        }
    );
    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::draw;
        }, 
        [&](const Event& event) -> void {
            manager.resize(manager_drawer.get_heigth());
            manager.reload();
            // manager.draw();
        }
    );
}

App::Circle_res App::circle() {
    player_drawer.set(
        0, interface.get_height() - player.get_height(),
        // 0, 10,
        interface.get_width(), player.get_height()
    );
    manager_drawer.set(
        0, 0,
        // interface.get_width(), interface.get_height() - player.get_height()
        interface.get_width(), 10
    );
    interface.set_color(setup.colors.def);

    interface.cls();

    event_queue.push_event(Event::create_draw());
    while (!event_queue.empty()) {
        event_queue.pop_event();
    }

    interface.update();

    return Circle_res::success;
}

void App::run() {

    while (true) {
        auto time_start = std::chrono::steady_clock::now();
        auto time_period = std::chrono::milliseconds(1000/20);
        Circle_res res = circle();
        if (res == Circle_res::exit) {
            break;
        }
        std::this_thread::sleep_until(time_start + time_period);
    }

}
