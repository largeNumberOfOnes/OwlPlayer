#include "../src/had.h"
#include <iostream>
#include <thread>

int test1() {
    had::start_window();
    had::end_window();
    return 0;
}

int test2() { // Test color
    had::start_window();

    had::Color col;
    had::set_new_color(col, 100, 0, 200);
    had::ColorPair pair;
    had::set_new_color_pair(pair, col, had::Colors::black);
    had::use_color_pair(pair);
    had::draw_text(0, 0, "Press q to continue");
    had::update();
    while (had::get_key() != had::key::q) {}

    had::end_window();
    return 0;
}

int test3() {
    had::audio_device_ctor();
    had::audio_device_dtor();

    std::cout << "test3" << std::endl;
    had::audio_device_ctor();

    if (had::load("/home/dt/Documents/audioPlayer/data/aud.wav")) {
        log_err("Cannot load");
    }
    res ret;
    while ((ret = had::player_update()) == res::success) {}

    had::remove();

    had::audio_device_dtor();

    std::cout << "starting while(true)" << std::endl;
    while (true) {}

    return 0;
}

int test4() {
    std::cout << "test3" << std::endl;
    had::audio_device_ctor();
    had::start_window();

    if (had::load("/home/dt/Documents/audioPlayer/data/aud.wav")) {
        log_err("Cannot load");
    }
    res ret;
    // while (true) {
    //     res ret = had::player_update();
    //     if (ret == res::error) {
    //         break;
    //     }
    // }
    // ret = had::player_update();
    std::thread newThread(had::player_update());
    // if (ret == res::error) {
    //     log_err("connot load file");
    //     // break;
    // }
    std::cout << "End of writing" << std::endl;

    while (true) {
        had::key key = had::get_key();
        if (key == had::key::space) {
            had::play_stop_audio();
        }

        had::update();
    }

    had::remove();

    had::end_window();
    had::audio_device_dtor();

    std::cout << "starting while(true)" << std::endl;
    while (true) {}

    return 0;
}

int main() {

    test1();
    // test2();
    // test3();
    test4();

    return 0;
}
