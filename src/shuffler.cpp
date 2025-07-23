#include "shuffler.h"

#include <optional>
#include <random>



Shuffler::Shuffler(FileManager& manager, QueuePanel& queue,
                                                    const had::Logger& log)
    : manager(manager)
    , queue(queue)
    , log(log)
{}

std::optional<std::string> Shuffler::get_sequential() {
    if (seq_pointer < seq_vec.size()) {
        ++seq_pointer;
        return seq_vec[seq_pointer];
    } else {
        return std::nullopt;
    }
}

std::optional<std::string> Shuffler::get_shuffle() {
    static std::mt19937 engine = []() -> std::mt19937 {
        std::mt19937 engine;
        std::random_device device;
        engine.seed(device());
        return engine;
    }();

    if (seq_vec.size() == 0) {
        log.log_info("no composition list");
        return std::nullopt;
    }
    std::uniform_int_distribution<int> distribution(0, seq_vec.size() - 1);
    return seq_vec[distribution(engine)];
}

had::Res Shuffler::set_sequential(std::vector<std::string>&& vec) {
    seq_vec = std::move(vec);
    seq_pointer = 0;
    return had::Res::success;
}

std::optional<std::string> Shuffler::get_next_composition() {
    switch(mode) {
        case Mode::none : return std::nullopt;
        case Mode::queue: return queue.pop();
        case Mode::sequential: return get_sequential();
        case Mode::shuffle: return get_shuffle();
    }
}

void Shuffler::rotate_modes_cicle() {
    mode = [mode = mode]() {
        switch (mode) {
            case Mode::none      : return Mode::shuffle   ;
            case Mode::shuffle   : return Mode::queue     ;
            case Mode::queue     : return Mode::sequential;
            case Mode::sequential: return Mode::none      ;
        };
    }();
}

std::string Shuffler::mode_to_str(Mode mode) {
    switch (mode) {
        case Mode::none      : return "None"      ;
        case Mode::shuffle   : return "Shuffle"   ;
        case Mode::queue     : return "Queue"     ;
        case Mode::sequential: return "Sequential";
    };
}

std::optional<Shuffler::Mode> Shuffler::str_to_mode(std::string_view str) {
    if (str == "None"      ) { return Mode::none      ; }
    if (str == "Shuffle"   ) { return Mode::shuffle   ; }
    if (str == "Queue"     ) { return Mode::queue     ; }
    if (str == "Sequential") { return Mode::sequential; }
    return std::nullopt;
}

std::string Shuffler::get_mode_str() {
    return mode_to_str(mode);
}

had::Res Shuffler::set_mode(Mode mode) {
    this->mode = mode;
    return had::Res::success;
}
