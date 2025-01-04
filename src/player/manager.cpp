#include "manager.h"
#include "abstract.h"
#include "interface.h"
#include "player.h"
#include <curses.h>
#include <string>



Manager::Manager(Abstract& abs, Player& player, Interface& interface)
    : abs(abs)
    , player(player)
    , interface(interface)
{
    key_table[key::arrow_left] = &Manager::volume_down;
    key_table[key::arrow_rigth] = &Manager::volume_up;
}

void Manager::volume_up() {
    interface.set_vol(interface.get_vol()+10);
}

void Manager::volume_down() {
    interface.set_vol(interface.get_vol()-10);
}

manager_ret Manager::work() {
    
    key k = abs.get_key();

    // if (key_table.count(k)) {
    //     (this->*key_table[k])();
    // }
    if (auto it = key_table.find(k); it != key_table.end()) {
        (this->*(it->second))();
    }

    if (interface.draw()) {
        std::cerr << "Error: cannot draw interface:" << std::endl;
        return manager_ret::error;
    }

    return manager_ret::success;
}
