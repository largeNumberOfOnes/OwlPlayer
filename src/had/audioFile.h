/*
 * This is a class representing access to the raw data of an audio file.
 *    The class is designed to be part of 'had(./had.h)' interface.
 */
 
#pragma once

#include "../glob_types.h"
#include <cstddef>


class AudioFile {
    int fd = -1;
    unsigned int rate = 0;
    int channels = 0;
    int samples = 0;
    bool is_init = false;

    res read_wav(char const* path);
    res read_mp3(char const* path);

    public:
        res init(char const* path);
        res dstr();
        AudioFile(char const* path, res& err);
        AudioFile();
        ~AudioFile();

        int get_rate();
        int get_channels();
        int get_samples();

        res read_file(void* buf, size_t count, size_t& retcount);
};
