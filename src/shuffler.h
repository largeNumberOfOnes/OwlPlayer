/**
 * pass
 */

#pragma once

#include "fileManager.h"
#include "had/had_logger.h"
#include "had/had_types.h"
#include "queuePanel.h"
#include <string_view>
#include <vector>

class Shuffler {
    std::vector<std::string> seq_vec;
    int seq_pointer;

    const had::Logger& log;
    FileManager& manager;
    QueuePanel& queue;
    std::optional<std::string> get_sequential();
    std::optional<std::string> get_shuffle();

    public:
        enum class Mode {
            none,
            shuffle,
            queue,
            sequential,
        };

    private:
        Mode mode = Mode::sequential;

    public:
        Shuffler(FileManager& manager, QueuePanel& queue,
                                                const had::Logger& log);
        Shuffler(const Shuffler&) = delete;

        void rotate_modes_cicle();
        std::string get_mode_str();
        static std::optional<Shuffler::Mode> str_to_mode(
                                                    std::string_view str);
        static std::string mode_to_str(Mode mode);
        had::Res set_sequential(std::vector<std::string>&& vec);
        had::Res set_mode(Mode mode);

        std::optional<std::string> get_next_composition();
};
