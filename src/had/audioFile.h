/*
 * This is a class representing access to the raw data of an audio file.
 *    The class is designed to be part of 'had(./had.h)' interface.
 */
 
#pragma once

#include <functional>
#include <string>
#include <cstddef>

#include "had_logger.h"
#include "had_types.h"

namespace had {
    class AudioFile {
        bool is_inited      = false;
        bool is_end_reached = false;
        std::size_t cur_pos = 0;
        FILE* file          = nullptr;
        int rate     = 0;
        int channels = 0;
        int samples  = 0;

        const Logger& log;

        Res read_wav(char const* path);
        Res read_mp3(char const* path);

        int buf_size = 0;
        int* buf = nullptr;
        void copy_to_buf(short* ref_buf, int size);

        public:
            enum class res_code {
                success,
                end_of_file,
                not_initialized,
                cannot_set_position,
                file_does_not_exist,
                other_error,
            };

            res_code load(const std::string& path);
            res_code erase();
            AudioFile(const Logger& log);
            ~AudioFile();

            std::size_t get_cur_pos();
            std::size_t get_max_pos();
            int get_rate();
            int get_channels();
            int get_samples();

            res_code read_file(void* buf, std::size_t count,
                                                    std::size_t& retcount);
            res_code set_position(std::size_t position);
    };
}
