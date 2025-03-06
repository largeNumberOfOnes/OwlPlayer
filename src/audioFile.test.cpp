#include "audioFile.h"

#include <iostream>
#include <assert.h>



int test1() { // testing wav load
    res err;
    AudioFile file{"/home/dt/Documents/audioPlayer/data/alyans_na_zare.wav", err};
    assert(!err);

    std::cout << "Testing wav:" << std::endl;
    std::cout << "  rate: " << file.get_rate() << std::endl;
    std::cout << "  channels: " << file.get_channels() << std::endl;
    std::cout << "  samples: " << file.get_samples() << std::endl;

    return 0;
}

int test2() { // testing wav load
    res err;
    AudioFile file{"/home/dt/Documents/audioPlayer/data/alyans_na_zare.mp3", err};
    assert(!err);

    std::cout << "Testing mp3:" << std::endl;
    std::cout << "  rate: " << file.get_rate() << std::endl;
    std::cout << "  channels: " << file.get_channels() << std::endl;
    std::cout << "  samples: " << file.get_samples() << std::endl;

    return 0;
}

int main() {

    test1();
    test2();

    return 0;
}
