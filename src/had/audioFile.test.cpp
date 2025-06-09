#include "audioFile.h"
#include "had_logger.h"

#include <iostream>
#include <assert.h>



int test1() { // testing wav load
    had::AudioFile file{stdlogger};
    had::AudioFile::res_code err = file.load(
        "/home/dt/Documents/audioPlayer/data/alyans_na_zare.wav");
    assert(err == had::AudioFile::res_code::success);

    std::cout << "Testing wav:" << std::endl;
    std::cout << "  rate: " << file.get_rate() << std::endl;
    std::cout << "  channels: " << file.get_channels() << std::endl;
    std::cout << "  samples: " << file.get_samples() << std::endl;

    return 0;
}

int test2() { // testing mp3 load
    had::AudioFile file{stdlogger};
    had::AudioFile::res_code err = file.load(
        "/home/dt/Documents/audioPlayer/data/alyans_na_zare.mp3");
    assert(err == had::AudioFile::res_code::success);

    std::cout << "Testing mp3:" << std::endl;
    std::cout << "  rate: " << file.get_rate() << std::endl;
    std::cout << "  channels: " << file.get_channels() << std::endl;
    std::cout << "  samples: " << file.get_samples() << std::endl;

    return 0;
}

int test3() { // testing init
    had::AudioFile file{stdlogger};
    had::AudioFile::res_code err = file.load(
        "/home/dt/Documents/audioPlayer/data/alyans_na_zare.mp3");
    assert(err == had::AudioFile::res_code::success);

    err = file.load("/home/dt/Documents/audioPlayer/data/alyans_na_zare.mp3");
    assert(err == had::AudioFile::res_code::success);

    err = file.set_position(file.get_samples() / 10);
    assert(err == had::AudioFile::res_code::success);

    return 0;
}

int main() {

    test1();
    test2();
    test3();

    return 0;
}
