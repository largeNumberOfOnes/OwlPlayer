#include "switchPanel.h"



SwitchPanel::SwitchPanel() {

}


void SwitchPanel::inc() {
    ++cur;
    cur %= count;
}

void SwitchPanel::dec() {
    --cur;
    cur %= count;
}

void SwitchPanel::set(int ch) {
    cur = ch;
    cur %= count;
}
