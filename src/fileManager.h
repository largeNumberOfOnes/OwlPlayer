#pragma once

#include "had/had.h"
#include <cstddef>
#include <vector>
#include <filesystem>

#include "had/had_interface.h"
#include "had/had_logger.h"
#include "setup.h"



class FileManager {
    static had::Dem const min_w = 14;
    static had::Dem const line_offset = 3;
    static had::Dem const free_space = 5;

    static had::Dem const draw_line_buf_size = 1024;
    char* draw_line_buf = nullptr;
    // static inline had::dem heigth_to_size(had::dem h) { return h - 2; }

    had::Drawer& drawer;
    const had::Logger& log;
    const Setup& setup;

    std::string dir;
    had::Dem pointer = 0;
    had::Dem top = 0;
    had::Dem size = 0;
    had::Dem list_size = 0;
    std::vector<std::filesystem::directory_entry> list;

    FileManager& operator=(FileManager const&) = delete;
    FileManager(FileManager const&) = delete;

    public:
        static had::Dem const min_h = 3;
        
        FileManager(std::string dir, had::Drawer& drawer,
                            const Setup& setup, const had::Logger& log);
        ~FileManager();

        had::Res go();
        had::Res up();
        had::Res down();
        had::Res reload();
        had::Res resize(had::Dem newh);

        bool is_enougth_space(had::Dem w, had::Dem h);
        had::Res draw();

};
