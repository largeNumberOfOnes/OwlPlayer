


#include "had_audio.h"
#include "had_logger.h"
#include "had_types.h"
#include <cassert>
#include <chrono>
#include <thread>



int main() {


    had::Res result;
    had::Audio audio{result, had::stdlogger};

    // audio.load("/home/dt/Documents/audioPlayer/data/alyans_na_zare.mp3");
    // audio.load("/home/dt/Documents/audioPlayer/data/Korn - Lost In The Grandeur.mp3");
    // audio.load("/home/dt/Desktop/mus/TOOL - Pneuma (Audio).mp3");
    // audio.load("/home/dt/Desktop/mus/mus2/Falling In Reverse - Ronald (feat. Tech N9ne & Alex Terrible).mp3");
    // audio.load("/home/dt/Desktop/mus/Disturbed - Decadence [Official Audio].mp3");
    had::Audio::res_code ret = 
        audio.load("/home/dt/Desktop/mus/Applause From Below.mp3");
    assert(ret == had::Audio::res_code::success);
    audio.play();

    std::cout << "Waiting for 100 seconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // std::cout << "stop" << std::endl;
    // audio.stop();
    // // std::this_thread::sleep_for(std::chrono::seconds(5));
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // std::cout << "cur time: " << audio.get_cur_time() << std::endl;
    // std::cout << "duration: " << audio.get_duration() << std::endl;

    // int tim = 100;
    // std::cout << "play" << std::endl;
    // audio.play();
    // std::this_thread::sleep_for(std::chrono::milliseconds(tim));
    // std::cout << "stop" << std::endl;
    // audio.stop();
    // std::this_thread::sleep_for(std::chrono::milliseconds(tim));
    // std::cout << "play" << std::endl;
    // audio.play();
    // std::this_thread::sleep_for(std::chrono::milliseconds(tim));
    // std::cout << "stop" << std::endl;
    // audio.stop();
    // std::this_thread::sleep_for(std::chrono::milliseconds(tim));
    // std::cout << "play" << std::endl;
    // audio.play();
    // std::this_thread::sleep_for(std::chrono::milliseconds(tim));
    // std::cout << "stop" << std::endl;
    // audio.stop();
    // std::this_thread::sleep_for(std::chrono::milliseconds(tim));
    // std::cout << "play" << std::endl;
    // audio.play();
    // std::this_thread::sleep_for(std::chrono::milliseconds(tim));
    // std::cout << "stop" << std::endl;
    // audio.stop();
    // std::this_thread::sleep_for(std::chrono::milliseconds(tim));

    // std::cout << "play" << std::endl;
    // audio.play();
    // std::this_thread::sleep_for(std::chrono::seconds(5));

    // std::cout << "vol = 25%" << std::endl;
    // audio.set_volume(25);
    // std::this_thread::sleep_for(std::chrono::seconds(5));
    // std::cout << "vol = 50%" << std::endl;
    // audio.set_volume(50);
    // std::this_thread::sleep_for(std::chrono::seconds(5));
    // std::cout << "vol = 75%" << std::endl;
    // audio.set_volume(75);
    // std::this_thread::sleep_for(std::chrono::seconds(5));
    // std::cout << "vol = 100%" << std::endl;
    // audio.set_volume(100);
    // std::this_thread::sleep_for(std::chrono::seconds(5));

    // std::cout << "vol = 200%" << std::endl;
    // audio.set_volume(200);
    // std::this_thread::sleep_for(std::chrono::seconds(5));

    // std::cout << "vol = 100%, jump on 2:37" << std::endl;
    // audio.set_volume(100);
    // audio.jump(157);
    // std::this_thread::sleep_for(std::chrono::seconds(5));
    // std::cout << "jump 1s backward" << std::endl;
    // audio.jump_rel(-1);
    // std::this_thread::sleep_for(std::chrono::seconds(5));
    // std::cout << "jump 30s bacward" << std::endl;
    // audio.jump_rel(-30);
    // std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "jump on 3:55" << std::endl;
    audio.jump(3*60 + 58);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // audio.play();
    // std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "playing other" << std::endl;
    ret = audio.load("/home/dt/Desktop/mus/Disturbed - Decadence [Official Audio].mp3");
    assert(ret == had::Audio::res_code::success);
    audio.play();

    std::this_thread::sleep_for(std::chrono::seconds(60*5));

    return 0;
}