


#include "had_audio.h"
#include "had_logger.h"
#include "had_types.h"
#include <chrono>
#include <thread>



int main() {


    had::res result;
    had::Audio audio{result, stdlogger};

    // audio.load("/home/dt/Documents/audioPlayer/data/alyans_na_zare.mp3");
    // audio.load("/home/dt/Documents/audioPlayer/data/Korn - Lost In The Grandeur.mp3");
    // audio.load("/home/dt/Desktop/mus/TOOL - Pneuma (Audio).mp3");
    // audio.load("/home/dt/Desktop/mus/mus2/Falling In Reverse - Ronald (feat. Tech N9ne & Alex Terrible).mp3");
    // audio.load("/home/dt/Desktop/mus/Disturbed - Decadence [Official Audio].mp3");
    audio.load("/home/dt/Desktop/mus/Applause From Below.mp3");
    // audio.play();


    std::cout << "Waiting for 100 seconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // std::cout << "stop" << std::endl;
    // audio.stop();
    // std::this_thread::sleep_for(std::chrono::seconds(5));

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

    std::cout << "vol = 100%, jump 1s backward" << std::endl;
    audio.set_volume(100);
    audio.jump_rel(-1);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "jump on 2:37" << std::endl;
    audio.jump(157);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "jump 30s bacward" << std::endl;
    audio.jump_rel(-30);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::this_thread::sleep_for(std::chrono::seconds(60));


    return 0;
}