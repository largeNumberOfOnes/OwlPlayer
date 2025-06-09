#include "eventQueue.hpp"




void ctor_and_dtor() {
    EventQueue queue;
}


void test() {
    EventQueue queue;

    queue.push_event(EventKeyPress{'3'});

}

int main() {

    ctor_and_dtor();
    test();

    return 0;
}
