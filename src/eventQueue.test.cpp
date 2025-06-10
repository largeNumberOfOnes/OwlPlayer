#include "eventQueue.hpp"

#include "had/had_keys.h"
#include <cassert>



void ctor_and_dtor() {
    EventQueue queue;
}

void test() {
    EventQueue queue;

    int arr[10] = {0};
    int ans[10] = {2, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    queue.push_event(Event::create_keypress(had::Key::arrow_down));
    queue.push_event(Event::create_keypress(had::Key::arrow_left));
    queue.push_event(Event::create_keypress(had::Key::arrow_rigth));
    queue.push_event(Event::create_draw());
    queue.push_event(Event::create_keypress(had::Key::space));
    queue.push_event(Event::create_keypress(had::Key::q));

    queue.add_oserver(
        [](const Event& event) -> bool {
            return (event.type == Event::EventType::keypress
                && event.key == had::Key::arrow_left)
                || event.type == Event::EventType::draw;
        },
        [&](const Event&) mutable -> void {
            arr[0]++; arr[1]++; }
    );
    queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::keypress
                && event.key == had::Key::arrow_down;
        },
        [&](const Event&) mutable -> void { arr[2] = 3; }
    );
    queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::keypress
                && event.key == had::Key::arrow_rigth;
        },
        [&](const Event&) mutable -> void { arr[3] = 4; }
    );
    queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::keypress
                && event.key == had::Key::space;
        },
        [&](const Event&) mutable -> void { arr[4] = 5; }
    );
    queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::keypress
                && event.key == had::Key::q;
        },
        [&](const Event&) mutable -> void { arr[5] = 6; }
    );
    queue.add_oserver(
        [](const Event& event) -> bool {
            return event.type == Event::EventType::draw;
        },
        [&](const Event&) mutable -> void { arr[6] = 7; }
    );

    while (!queue.empty()) {
        queue.pop_event();
    }

    for (int q = 0; q < 7; ++q) {
        assert(arr[q] == ans[q]);
    }
}

int main() {

    ctor_and_dtor();
    test();

    return 0;
}
