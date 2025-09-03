/*
 * This is a class representing access to the raw data of an audio file.
 *    The class is designed to be part of 'had(./had.h)' interface.
 */
 
#pragma once

#include <string_view>
#include <functional>
#include <optional>
#include <complex>
#include <string>
#include <vector>

#include "had_logger.h"
#include "had_types.h"
#include "had_dataFrame.h"



namespace had {
    static constexpr bool PRINT_FFMPEG_WARNINGS = false;

    class AudioFile {

        bool is_inited      = false;
        bool is_end_reached = false;
        SampleDem cur_pos   = 0;
        FILE* file          = nullptr;
        int rate            = 0;
        int channels        = 0;
        SampleDem samples   = 0;
        const Logger& log;

        Res convert_to_pcm(
            const char* input_file, const char* output_file
        );

        class FrameManager {
            static std::size_t get_id();
            std::size_t id = get_id();
            DataFrame frame1{id};
            DataFrame frame2{id};
            enum class OutFrame {
                FIRST,
                SECOND,
            } out_frame = OutFrame::FIRST;
            bool is_frame_issued = false;
            bool is_in_frame_ready = false;
            FrameManager();
            DataFrame& get_in_frame_ref();
            DataFrame& get_out_frame_ref();
            void swap_out_frame();
            public:
                void return_frame(DataFrame&& frame);
                DataFrame& get_in_frame();
                std::optional<DataFrame> get_out_frame(); // std::nullopt
                                        // if frame was already issued
                DataFrame update_out_frame(DataFrame&& frame);
                void mark_in_frame_as_ready();
        } frame_manager;
        void copy_to_buf(short* ref_buf, SampleDem samples);

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

            Res load(std::string_view path);
            void erase();

            SampleDem get_cur_pos();
            SampleDem get_max_pos();
            int get_rate();
            int get_channels();
            int get_samples();

            res_code read_file(void* buf, SampleDem samples,
                                                    SampleDem& retcount);
            res_code set_position(SampleDem position);

            std::optional<DataFrame> get_first_frame(); // Rerurns frame
                                // only once. std::nullopt in other times.
            DataFrame get_frame(DataFrame&& frame);

            SampleDem byte_to_samples(std::size_t bytes);
            std::size_t samples_to_byte(SampleDem pos);
            SampleDem seconds_to_samples(seconds time);
            seconds samples_to_seconds(SampleDem samples);
    };
}
