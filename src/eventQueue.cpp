#include "eventQueue.h"
#include <memory>
#include <utility>



void EventQueue::push_event(Event&& event) {
    events.push(std::make_unique<Event>(std::move(event)));
}

void EventQueue::pop_event() {
    if (events.empty()) {
        return;
    }

    std::unique_ptr<Event> event_ptr = std::move(events.front());
    events.pop();

    for (auto pair : observers) {
        if (pair.first(*event_ptr.get())) {
            pair.second(*event_ptr.get());
        }
    }
}

bool EventQueue::empty() const {
    return events.empty();
}

void EventQueue::add_observer(
    std::function<bool(const Event&)> comp,
    std::function<void(const Event&)> observer
) {
    observers.push_back(
        std::pair<
            std::function<bool(const Event&)>,
            std::function<void(const Event&)>
        >(comp, observer)
    );
}

void EventQueue::unwrap() {
    while (!empty()) {
        pop_event();
    }
}
