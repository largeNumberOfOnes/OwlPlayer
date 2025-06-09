#include "had.h"
#include "../glob_types.h"

#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <ostream>
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

int test_audio_turn_on_off() { // 
    res err;
    err = had::aud_start();
    assert(!err);
    err = had::aud_end();
    assert(!err);
    
    return 0;
}

int test_AudioPlayer_run_mp3() {
    std::cout << 
        "test plan:\n"
        "  load and start playback\n"
        "  wait 10 seconds and pause\n"
        "  wait 2 seconds and continue playback\n"
        "  play 2 seconds with volume = 100\n"
        "  play 2 seconds with volume = 70\n"
        "  play 2 seconds with volume = 40\n"
        "  play 2 seconds with volume = 10\n"
        "  palay 10 seconds with volume = 100\n"
        "  wait 10 seconds and close the player\n";

    res err = had::aud_start();
    assert(!err);

    err = had::aud_load(
                "/home/dt/Documents/audioPlayer/data/alyans_na_zare.mp3");
    assert(!err);

    err = had::aud_run();
    assert(!err);
    deb_waiting_for(10, "play");

    err = had::aud_play_stop();
    assert(!err);
    deb_waiting_for(2, "stop");

    err = had::aud_set_volume(100);
    assert(!err);
    err = had::aud_play_stop();
    assert(!err);
    deb_waiting_for(2, "play, volume = 100");
    
    err = had::aud_set_volume(70);
    assert(!err);
    deb_waiting_for(2, "play, volume = 70");

    err = had::aud_set_volume(40);
    assert(!err);
    deb_waiting_for(2, "play, volume = 40");

    err = had::aud_set_volume(10);
    assert(!err);
    deb_waiting_for(2, "play, volume = 10");

    err = had::aud_set_volume(100);
    assert(!err);
    deb_waiting_for(1000, "play");

    err = had::aud_drop();
    assert(!err);
    err = had::aud_end();
    assert(!err);

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
