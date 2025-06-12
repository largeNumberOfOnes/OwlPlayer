
#include "had_interface.h"
#include "had_keys.h"
#include "had_logger.h"
#include "had_types.h"
#include <chrono>
#include <string>
#include <thread>



int main() {
    had::Logger log;
    had::Interface interface{log};
    had::Dem w = interface.get_width();
    had::Dem h = interface.get_height();
    had::Drawer drawer{interface, 0, 0, w, h, log};

    log.log_err  = [&](std::string mes) { drawer.draw_text(0, h - 4, "Err:  " + mes); };
    log.log_warn = [&](std::string mes) { drawer.draw_text(0, h - 3, "Warn: " + mes); };
    log.log_info = [&](std::string mes) { drawer.draw_text(0, h - 2, "Info: " + mes); };

    int frame = 0;
    std::string last_seq_str = "";
    while (true) {
        had::KeySequence seq = interface.catch_key_seq();
        std::string seq_str = seq.to_str();
        if (!seq.is_empty()) {
            last_seq_str = seq_str;
        }

        std::string cls_str;
        for (int q = 0; q < w; ++q) {
            cls_str += ' ';
        }


        drawer.draw_text(0, 0, cls_str);
        drawer.draw_text(0, 1, cls_str);
        drawer.draw_text(0, 0, "cur key seq: "  + seq_str);
        drawer.draw_text(0, 1, "last key seq: " + last_seq_str);

        drawer.draw_text(0, h - 1, cls_str);
        drawer.draw_text(0, h - 1, "frame: " + std::to_string(frame++));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
