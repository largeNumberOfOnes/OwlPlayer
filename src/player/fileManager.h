#pragma once

#include "../had.h"
#include <cstddef>
#include <vector>
#include <filesystem>

#include "../glob_types.h"



class FileManager {
    static had::dem const min_w = 14;
    static had::dem const line_offset = 3;
    static had::dem const free_space = 5;

    static had::dem const draw_line_buf_size = 1024;
    char* draw_line_buf = nullptr;
    // static inline had::dem heigth_to_size(had::dem h) { return h - 2; }

    std::string dir;
    had::dem pointer = 0;
    had::dem top = 0;
    had::dem size = 0;
    had::dem list_size = 0;
    std::vector<std::filesystem::directory_entry> list;

    FileManager& operator=(FileManager const&) = delete;
    FileManager(FileManager const&) = delete;

    public:
        static had::dem const min_h = 3;
        
        FileManager(std::string dir);
        ~FileManager();

        res go();
        res up();
        res down();
        res reload();
        res resize(had::dem newh);

        res draw(had::dem x, had::dem y, had::dem w, had::dem h);

};
