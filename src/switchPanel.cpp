#include "switchPanel.h"



SwitchPanel::SwitchPanel() {

}

SwitchPanel::~SwitchPanel() {

}

void SwitchPanel::inc() {
    set(cur + 1);
}

void SwitchPanel::dec() {
    set(cur - 1);
}

void SwitchPanel::set(int ch) {
    cur = ch;
    cur %= count;
    component_list[cur].resize();
}

SwitchPanel::ComponentId SwitchPanel::add_component(
    std::function<had::Res(void)> draw,
    std::function<had::Res(void)> resize
) {
    component_list.push_back(
        Component{draw, resize}
    );
    ++count;

    return ComponentId{count - 1};
}

had::Res SwitchPanel::draw() {
    return component_list[cur].draw();
}

had::Res SwitchPanel::resize() {
    return component_list[cur].resize();
}

template <typename Ret, typename... Args>
Ret SwitchPanel::call_wrapper(
    ComponentId id,
    std::function<Ret(Args... args)> func,
    Ret default_ret,
    Args... args
) {
    if (compare_id(id)) {
        return func(std::forward(args...));
    }
    return default_ret;
}

void SwitchPanel::call_wrapper(
    ComponentId id,
    std::function<void(void)> func
) {
    if (compare_id(id)) {
        return func();
    }
}
