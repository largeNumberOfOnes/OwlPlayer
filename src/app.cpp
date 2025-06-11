#include "app.h"

#include "had/had.h"
#include "player.h"

#include <chrono>
#include <iostream>
#include <thread>



App::App(const had::Logger& log)
    : log(log)
    , player_drawer(interface, 0, 0, 0, 0, log)
    , player(player_drawer, log)
{
    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::draw;
        }, 
        [&](const Event& event) -> void {
            player.draw();
        }
    );
}

App::Circle_res App::circle() {
    event_queue.push_event(Event::create_draw());

    player_drawer.set(0, 0, interface.get_width(), interface.get_height());

    interface.cls();

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
