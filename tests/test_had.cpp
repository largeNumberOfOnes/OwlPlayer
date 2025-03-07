#include "../src/had.h"
#include "../src/glob_types.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

// ########################################################################

int test_Interface_turn_on_off() {
    had::start_window();
    had::end_window();
    return 0;
}

int test_Interface_colors() {
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

int test_Interface() {
    TEST(test_Interface_turn_on_off);
    TEST(test_Interface_colors);

    return 0;
}

// ########################################################################

int test_AudioPlayer_turn_on_off() { // 
    res err;
    err = had::aud_start();
    assert(!err);
    err = had::aud_end();
    assert(!err);
    
    return 0;
}

int test_AudioPlayer_run_mp3() {
    had::aud_start();

    res err = had::aud_load(
                "/home/dt/Documents/audioPlayer/data/alyans_na_zare.mp3");
    assert(!err);
    had::aud_run();

    std::cout << "sleeping..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1000));
    while (true) {}


    
    had::aud_end();

    return 0;
}

int test_AudioPlayer() {
    // TEST(test_AudioPlayer_turn_on_off);
    TEST(test_AudioPlayer_run_mp3);

    return 0;
}

// ########################################################################

int main() {

    // TEST(test_Interface);
    TEST(test_AudioPlayer);

    return 0;
}
