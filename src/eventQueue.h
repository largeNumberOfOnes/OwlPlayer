

#include <functional>
#include <memory>
#include <vector>
#include <queue>

#include "had/had.h"

class Event {
    Event() {}
    public:
        enum class EventType {
            keypress,
            draw,
        } type;

        had::KeySequence seq = had::KeySequence::create_empty();

        static Event create_keypress(had::KeySequence seq) {
            Event e;
            e.type = EventType::keypress;
            e.seq = seq;
            return e;
        }
        static Event create_draw() {
            Event e;
            e.type = EventType::draw;
            return e;
        }
};

class EventQueue {
    std::queue<std::unique_ptr<Event>> events;
    std::vector<std::pair<
        std::function<bool(const Event&)>,
        std::function<void(const Event&)>
    >> observers;

    public:
        void push_event(Event&& event);
        void pop_event();
        bool empty() const;
        void add_oserver(
            std::function<bool(const Event&)> comp,
            std::function<void(const Event&)> observer
        );

};
