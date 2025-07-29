#include "had_interface.h"
#include "had_keys.h"
#include "had_logger.h"
#include "had_types.h"

#include <chrono>
#include <list>
#include <string_view>
#include <string>
#include <thread>



int app1() {
    had::Logger log;
    had::Interface interface{log};
    had::Dem w = interface.get_width();
    had::Dem h = interface.get_height();
    had::Drawer drawer{interface, 0, 0, w, h, log};

    std::vector<std::string> list;
    log.log_err  = [&](std::string_view mes) {
        // drawer.draw_text(0, h - 4, "Err:  " + std::string(mes));
        list.push_back("Err:  " + std::string(mes));
    };
    log.log_warn = [&](std::string_view mes) {
        // drawer.draw_text(0, h - 3, "Warn: " + std::string(mes));
        list.push_back("Warn:  " + std::string(mes));
    };
    log.log_info = [&](std::string_view mes) {
        // drawer.draw_text(0, h - 2, "Info: " + std::string(mes));
        list.push_back("Info:  " + std::string(mes));
    };

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

        for (int q = 0; q < 5; ++q) {
            if (q < list.size()) {
                drawer.draw_text(0, h - q - 1, list[list.size() - 1]);    
            }
        }

        drawer.draw_text(0, h - 1, cls_str);
        drawer.draw_text(0, h - 1, "frame: " + std::to_string(frame++));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

int app2() {
    had::Logger log;
    had::Interface interface{log};
    had::Dem w = interface.get_width();
    had::Dem h = interface.get_height();
    had::Drawer drawer{interface, 0, 0, w, h, log};
    

    std::vector<std::string> list;
    log.log_err  = [&](std::string_view mes) {
        list.push_back("Err:  " + std::string(mes));
    };
    log.log_warn = [&](std::string_view mes) {
        list.push_back("Warn:  " + std::string(mes));
    };
    log.log_info = [&](std::string_view mes) {
        list.push_back("Info:  " + std::string(mes));
    };

    while (true) {
        // int ch = getch();
        interface.catch_key_seq();
        // std::cout << ch << std::endl;
        // if (ch != -1) {
        //     log.log_info(std::to_string(ch));
        // }

        for (int q = 0; q < h; ++q) {
            if (q < list.size()) {
                drawer.draw_text(0, h - q - 1, "                       ");    
                drawer.draw_text(0, h - q - 1, list[list.size() - 1 - q]);    
            }

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }


    return 0;
}

int main() {
    app1();

    return 0;
}
