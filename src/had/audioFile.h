/*
 * This is a class representing access to the raw data of an audio file.
 *    The class is designed to be part of 'had(./had.h)' interface.
 */
 
#pragma once

#include <string_view>
#include <functional>
#include <string>
#include <vector>

#include "had_logger.h"
#include "had_types.h"
#include "had_dataFrame.h"

#include "../../audio_decoders/dr_libs/dr_mp3.h"

namespace had {
    static constexpr bool PRINT_FFMPEG_WARNINGS = false;
    using SampleDem = std::size_t;
    using Value = int16_t;
    static_assert(sizeof(Value) == 2);

    class AudioFile {

        bool is_inited      = false;
        bool is_end_reached = false;
        SampleDem cur_pos = 0;
        FILE* file          = nullptr;
        int rate            = 0;
        int channels        = 0;
        SampleDem samples   = 0;
        const Logger& log;

        Res convert_to_pcm(const char* input_file, const char* output_file);

        std::vector<int> buf;
        void copy_to_buf(short* ref_buf, int size);

        public:
            AudioFile(const Logger& log);
            AudioFile(const AudioFile&)            = delete;
            AudioFile& operator=(const AudioFile&) = delete;
            ~AudioFile();

            enum class res_code {
                success,
                end_of_file,
                not_initialized,
                cannot_set_position,
                file_does_not_exist,
                other_error,
            };

            res_code load(std::string_view path);
            res_code erase();

            std::size_t get_cur_pos();
            std::size_t get_max_pos();
            int get_rate();
            int get_channels();
            int get_samples();

            res_code read_file(void* buf, std::size_t byte_count,
                                                    std::size_t& retcount);
            // res_code set_position(std::size_t position);
            res_code set_position(SampleDem position);

            [[deprecated]] const std::vector<int>& get_buf();
            DataFrame get_frame();

            SampleDem byte_to_samples(std::size_t bytes);
            std::size_t samples_to_byte(SampleDem pos);
            SampleDem seconds_to_samples(seconds time);
    };
}
