#pragma once

#include "abstract.h"
#include "interface.h"
#include "player.h"
#include "config.h"
#include <unordered_map>



enum class manager_ret {
    success,
    error,
    out,
};

class Manager {
    private:
        Abstract& abs;
        Player& player;
        Interface& interface;

        class TopPannel {
            bool is_active = false;
            std::string main_dir_name;
            // std::string main_dir_name;
        };
        TopPannel top;

        void volume_up();
        void volume_down();

        std::unordered_map<key, void (Manager::*)()> key_table;

    public:
        Manager(Abstract& abs, Player &player, Interface &interface);
        manager_ret work();
};
