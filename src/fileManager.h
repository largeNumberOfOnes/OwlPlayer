/**
 * Graphical file manager.
 */

#pragma once

#include "had/had.h"
class Setup; // DEV [Should include setup.h but this causes errors]

#include <string_view>
#include <filesystem>
#include <optional>
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
    std::optional<std::string> playing_file = std::nullopt;
    std::optional<had::Dem> playing_file_pointer = std::nullopt;
    std::function<void(std::string_view)> call_on_file;
    had::Dem selecter = 0;
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

    had::Res reload();
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
        FileManager(const FileManager&) = delete;
        ~FileManager();

        had::Res go();
        had::Res release();
        had::Res back();
        had::Res up();
        had::Res down();
        had::Res select(had::Dem new_pointer);
        had::Res set_playing_file(std::string_view path);
        had::Res resize();
        had::Res draw();

        std::vector<std::string> get_dirs_files();
        std::optional<std::string> get_selected_comp();

        void search_add_char(char ch);
        void search_set_string(std::string_view str);
        void search_clear_string();
};
