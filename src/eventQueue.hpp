

#include <functional>
#include <memory>
#include <vector>
#include <queue>

class Event {

};

class Event_KeyPress : public Event {
    // Key key;
};

class EventQueue {
    private:
        std::queue<std::unique_ptr<Event>> events;
        std::vector<std::pair<
            std::function<bool(const Event&)>,
            std::vector<std::function<void(const Event&)>>
        >> observers;

    public:
        void push_event(Event&& event);
        void pop_event();

};
