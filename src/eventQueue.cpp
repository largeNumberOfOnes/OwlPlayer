#include "eventQueue.hpp"
#include <memory>



void EventQueue::push_event(Event&& event) {
    events.push(std::make_unique<Event>(std::move(event)));
}

void EventQueue::pop_event() {
    // events.front();
}
