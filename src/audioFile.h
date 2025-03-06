#pragma once

#include "glob_types.h"


class AudioFile {
    int fd = -1;
    int rate = 0;
    int channels = 0;
    int samples = 0;

    res read_wav(char const* path);
    res read_mp3(char const* path);

    public:
        AudioFile(char const* path, res& err);
        ~AudioFile();

        int get_rate();
        int get_channels();
        int get_samples();

};
