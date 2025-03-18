#include "GUIPlayer.h"

#include <bits/types/FILE.h>
#include <curses.h>
#include <string>



GUIPlayer::GUIPlayer(char const* work_dir)
    : fileManager(work_dir)
    , player()
{
    fileManager.reload();
    action_list[had::key::arrow_up] = &GUIPlayer::fileManager_up;
    action_list[had::key::arrow_down] = &GUIPlayer::fileManager_down;
}

void GUIPlayer::fileManager_up() {
    fileManager.up();
}
void GUIPlayer::fileManager_down() {
    fileManager.down();
}

res GUIPlayer::start_loop() {

    if (had::start_window()) {
        log_err("Error while starting window");
    }
    if (intf::init_colors()) {
        log_err("Error while initializing colors");
    }

    while (true) {
        had::dem w = had::get_width();
        had::dem h = had::get_height();
        had::key k = had::get_key();
        
        had::cls();

        if (auto it = action_list.find(k); it != action_list.end()) {
            (this->*(it->second))();
        }

        if (player.is_enougth_space(w, h)) {
            if (player.draw(0, h - player.get_height(), w)) {
                log_err("Cannot draw player");
            }
        } else {
            had::draw_text(0, 0, "Not ehought space");
        }
        
        // if (!fileManager.is_enougth_space(w, h - player.get_height())) {
        //     had::draw_text(0, 0, "Not ehought space");
        // } else if (fileManager.draw(0, 0, w, h - player.get_height())) {
        //     log_err("Cannot draw fileManager");
        // }
    }

    if (had::end_window()) {
        log_err("Error while closing window");
    }

    return res::success;
}
