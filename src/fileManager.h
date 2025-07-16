/**
 * Graphical file manager.
 */

#pragma once

#include "had/had.h"
#include "had/had_types.h"
#include "setup.h"

#include <filesystem>
#include <cstddef>
#include <string_view>
#include <vector>



class FileManager {
    static constexpr had::Dem line_offset      = 4;
    static constexpr had::Dem line_free_space  = 5;
    static constexpr had::Dem min_w = line_offset + line_free_space + 5;
    static constexpr had::Dem min_h = 3;

    had::Drawer& drawer;
    const had::Logger& log;
    const Setup& setup;

    std::string dir;
    std::function<void(std::string)> call_on_file;
    had::Dem pointer = 0;
    had::Dem top = 0;
    had::Dem size = 0;
    had::Dem list_size = 0;

    struct File {
        bool reduce;
        std::size_t reduce_len;
        std::filesystem::directory_entry file;
    };
    std::vector<File> list;

    had::Res draw_tree_symbol(int q);
    had::Res draw_file_name(int q);
    had::Res draw_scrol_line();

    had::Res resize_width();
    had::Res resize_height();

    public:        
        FileManager(
            std::string dir, 
            std::function<void(std::string_view)> call_on_file,
            had::Drawer& drawer,
            const Setup& setup,
            const had::Logger& log
        );
        FileManager(FileManager const&) = delete;
        FileManager& operator=(FileManager const&) = delete;
        ~FileManager();

        had::Res go();
        had::Res back();
        had::Res up();
        had::Res down();
        // had::Res select();
        had::Res reload();
        had::Res resize();

        // bool is_mp3_file(had::Dem elem_number);
        // bool is_cur_mp3_file();
        // had::Dem get_cur_elem();
        // had::Dem get_files_count_in_dir();

        bool is_enougth_space(had::Dem w, had::Dem h);
        had::Res draw();

        void search_add_char(char ch);
        void search_set_string(std::string_view str);
        void search_clear_string();
};
