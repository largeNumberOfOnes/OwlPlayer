#include "shuffler.h"



std::string_view Shuffler::get_sequential() {
    // had::Dem cur = manager.get_cur_elem();
    // manager.down();
    // std::size_t q_max = manager.get_files_count_in_dir();
    // for (int q = 0; !manager.is_cur_mp3_file() && q < q_max; ++q) {}
    
    return "NONE"; // DEV
}

std::string_view Shuffler::get_shuffle() {
    return "NONE"; // DEV
}

std::optional<std::string_view> Shuffler::get_next_composition() {
    switch(mode) {
        case Mode::none : return std::nullopt;
        case Mode::queue: return queue.pop();
        case Mode::sequential: return get_sequential();
        case Mode::shuffle: return get_shuffle();
    }
}
