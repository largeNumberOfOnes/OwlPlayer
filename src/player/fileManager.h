#pragma once

#include "../abstract.h"
#include <cstddef>
#include <vector>
#include <filesystem>

#include "../glob_types.h"



class FileManager {
    Abstract& abs;

    std::string dir;
    std::size_t pointer;
    std::size_t top;
    std::size_t oldh;
    std::vector<std::filesystem::directory_entry> list;

    public:
        FileManager(Abstract& abs, std::string dir);

        stat go();
        stat up();
        stat down();
        stat reload();

        stat draw(dem x, dem y, dem w, dem h);

};
