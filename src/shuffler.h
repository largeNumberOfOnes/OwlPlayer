/**
 * pass
 */

#pragma once

#include "fileManager.h"
#include "queuePanel.h"

class Shuffler {
    FileManager& manager;
    QueuePanel& queue;
    std::string_view get_sequential();
    std::string_view get_shuffle();

    public:
        Shuffler(
            FileManager& manager,
            QueuePanel& queue
        ) : manager(manager), queue(queue) {}
        Shuffler(const Shuffler&) = delete;

        enum class Mode {
            none,
            shuffle,
            queue,
            sequential,
        } mode = Mode::sequential;

        std::optional<std::string_view> get_next_composition();
};
