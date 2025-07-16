#include "fileManager.h"

#include "had/had.h"
#include "had/had_interface.h"
#include "had/had_types.h"
#include "had/had_unicode.h"

#include <algorithm>

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>



FileManager::FileManager(
    std::string dir, 
    std::function<void(std::string_view)> call_on_file,
    had::Drawer& drawer,
    const Setup& setup,
    const had::Logger& log
)
    : dir(dir)
    , call_on_file(call_on_file)
    , drawer(drawer)
    , setup(setup)
    , log(log)
{
    if (!this->dir.ends_with('/')) {
        this->dir += '/';
    }
    reload();    
}

FileManager::~FileManager() {

}

had::Res FileManager::go() {
    if (list[pointer].file.is_directory()) {
        dir = list[pointer].file.path();
        dir += '/';
        reload();
    } else if (list[pointer].file.is_regular_file()) {
        std::string path = list[pointer].file.path();
        if (path.ends_with(".mp3")) {
            call_on_file(path);
        } // DEV [for temporary safety]
    }
    return had::Res::success;
}

had::Res FileManager::back() {
    // int ind = dir.find_last_of('/');
    if (dir != "/") {
        dir.pop_back();
        while (!dir.ends_with('/')) {
            dir.pop_back();
        }
        reload();
    }
    return had::Res::success;
}

had::Res FileManager::up() {
    if (list_size == 0)
        return had::Res::success;

    if (pointer == 0) {
        pointer = list_size - 1;
        if (size < list_size)
            top = list_size - size;
        else
            top = 0;
    } else {
        --pointer;
        if (pointer + 1 == top)
            --top;
    }

    return had::Res::success;
}

had::Res FileManager::down() {
    if (list_size == 0)
        return had::Res::success;

    if (pointer + 1 == list_size) {
        pointer = 0;
        top = 0;
    } else {
        ++pointer;
        if (pointer >= top + size)
            ++top;
    }

    return had::Res::success;
}

had::Res FileManager::select(had::Dem new_pointer) {
    if (list_size == 0) {
        return had::Res::success;
    }
    if (list_size <= new_pointer) {
        return had::Res::error;
    }
    if (list_size <= size) {
        pointer = new_pointer;
        return had::Res::success;
    }
    if (top <= new_pointer && new_pointer < top + size) {
        pointer = new_pointer;
        return had::Res::success;
    }
    if (new_pointer < size / 2) {
        top = 0;
        pointer = new_pointer;
    } else {
        top = new_pointer - size / 2;
        pointer = new_pointer;
        return had::Res::success;
    }
    return had::Res::error;
}

had::Res FileManager::reload() {
    list.clear();
    namespace fs = std::filesystem;

    // DEV [maybe there should be placed try-catch block]
    for (const auto& entry : fs::directory_iterator(dir)) {
        list.push_back(File{
            .reduce     = false,
            .reduce_len = 0,
            .file       = entry
        });
    }

    std::sort(
        list.begin(), list.end(),
        [](auto a, auto b) {
            if (a.file.is_directory() and b.file.is_directory()
                || a.file.is_regular_file() and b.file.is_regular_file()
            ) {
                return a.file.path() < b.file.path();
            } else if (a.file.is_directory()) {
                return true;
            } else {
                return false;
            }
        }
    );

    list_size = list.size();
    if (list_size == 0) {
        top = 0;
        pointer = 0;
    } else if (pointer >= list_size) {
        pointer = list_size - 1;
        if (list_size > size)
            top = pointer - size;
        else
            top = 0;
    }

    return had::Res::success;
}

had::Res FileManager::resize_width() {
    static had::Dem old_w = drawer.get_width();
    had::Dem w = drawer.get_width();
    if (old_w != w) {
        for (auto& it : list) {
            had::UnicodeStringView uni_str{
                it.file.path().filename().c_str()
            };
            std::size_t char_len = uni_str.get_char_len();
            it.reduce = (w < char_len + line_offset + line_free_space);
            if (it.reduce) {
                it.reduce_len = uni_str.get_substr_byte_len(
                    w - line_offset - line_free_space
                );
            }
        }
    } 
    return had::Res::success;
}

had::Res FileManager::resize_height() {
    size = drawer.get_height() - 2;
    if (pointer >= top + size) {
        if (size < list_size)
            top = pointer - size + 1;
        else
            top = 0;
    }
    if (list_size < top + size) {
        if (size < list_size)
            top = list_size - size + 1;
        else
            top = 0;
    }
    return had::Res::success;
}

had::Res FileManager::resize() {
    if (false
        || resize_width()
        || resize_height()
    ) {
        return had::Res::success;
    } else {
        return had::Res::error;
    }
}

bool FileManager::is_enougth_space(had::Dem w, had::Dem h) {
    return w >= min_w && h >= min_h;
}

had::Res FileManager::draw_tree_symbol(int q) {
    had::SpSymbol list_symbol = (top + q + 1 == list_size)
                              ? had::SpSymbol::list_end_symbol
                              : had::SpSymbol::list_symbol;
    if (false
        || drawer.draw_sp_symbol(1, q + 1, list_symbol)
        || drawer.draw_sp_symbol(2, q + 1, had::SpSymbol::list_line_symbol)
    ) {
        log.log_info("Cannot draw_tree_symbol");
        return had::Res::error;
    }
    return had::Res::success;
}

had::Res FileManager::draw_file_name(int q) {
    const File& list_elem = list[top+q];
    bool is_selected  = (top + q == pointer);
    bool is_directory = list_elem.file.is_directory();
    had::Color file_color =
          ( is_selected &&  is_directory) ? setup.colors.dir_selected
        : ( is_selected && !is_directory) ? setup.colors.file_selected
        : (!is_selected &&  is_directory) ? setup.colors.dir
        : (!is_selected && !is_directory) ? setup.colors.file
        : setup.colors.def;
    drawer.set_color(file_color);
    if (list_elem.reduce) {
        had::Dem w = drawer.get_width();
        drawer.draw_text_n(
            line_offset, q + 1,
            list_elem.file.path().filename().c_str(),
            list_elem.reduce_len
        );
        drawer.draw_text(w - line_free_space, q + 1, "... ");
    } else {
        drawer.draw_text(
            line_offset, q + 1, list_elem.file.path().filename().c_str()
        );
    }
    drawer.set_color(setup.colors.def);
    return had::Res::success;
}

had::Res FileManager::draw_scrol_line() {
    for (int q = 2; q < drawer.get_height() - 1; ++q) {
        drawer.draw_sp_symbol(
            drawer.get_width() - 1, q,
            had::SpSymbol::scrol_middle_symbol
        );
    }
    drawer.draw_sp_symbol(
        drawer.get_width() - 1, 1,
        had::SpSymbol::scrol_up_symbol
    );
    drawer.draw_sp_symbol(
        drawer.get_width() - 1, drawer.get_height() - 1,
        had::SpSymbol::scrol_bottom_symbol
    );

    return had::Res::success;
}

had::Res FileManager::draw() {
    had::Dem w = drawer.get_width();
    had::Dem h = drawer.get_height();
    if (w < min_w || h < min_h) {
        log.log_err("bad sizes");
        return had::Res::error;
    }
    drawer.cls();
    drawer.draw_text(0, 0, dir.c_str());
    std::size_t files_count = (size > list_size) ? list_size : size;
    for (std::size_t q = 0; q < files_count; ++q) {
        draw_tree_symbol(q);
        draw_file_name(q);
    }
    drawer.draw_symbol(0, h-1, ':');
    draw_scrol_line();
    return had::Res::success;
}

void FileManager::search_add_char(char ch) {
    log.log_info("FileManager::search_add_char");
}

void FileManager::search_set_string(std::string_view str) {
    log.log_info("FileManager::search_set_string");
}

void FileManager::search_clear_string() {
    log.log_info("FileManager::search_clear_string");
}

// bool FileManager::is_mp3_file(had::Dem elem_number) {
//     const std::filesystem::directory_entry& file = list[elem_number].file;
//     return file.is_regular_file()
//         && file.path().extension().compare("mp3"); // DEV [not strictly]
// }

// bool FileManager::is_cur_mp3_file() {
//     return is_mp3_file(pointer);
// }

// had::Dem FileManager::get_cur_elem() {
//     return pointer;
// }

// had::Dem FileManager::get_files_count_in_dir() {
//     return list_size;
// }
