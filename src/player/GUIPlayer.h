#pragma once

#include "../had.h"
#include "../intf.h"
#include "fileManager.h"
#include "player.h"
// #include "config.h"
#include <unordered_map>


class GUIPlayer {

    // struct Actions {
        void volume_up();
        void volume_down();

        void fileManager_up();
        void fileManager_down();
        void fileManager_select();
    // } actions;
    // std::unordered_map<had::key, void (Actions::*)()> action_list;
    std::unordered_map<had::key, void (GUIPlayer::*)()> action_list;

    FileManager fileManager;
    Player player;

    public:
        GUIPlayer(char const* work_dir);
        stat start_loop();
};
