


#include "app.h"






App::App() {

    event_queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::draw;
        }, 
        [](const Event& event) -> void {
            std::cout << "drawing" << std::endl;
        }
    );

}

App::Circle_res App::circle() {
    event_queue.push_event(Event::create_draw());

    while (!event_queue.empty()) {
        event_queue.pop_event();
    }

    return Circle_res::success;
}

void App::run() {

    while (true) {
        Circle_res res = circle();
        if (res == Circle_res::exit) {
            break;
        }
    }

}
