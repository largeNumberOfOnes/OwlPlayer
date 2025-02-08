#include "fileManager.h"
#include <cstddef>



FileManager::FileManager(Abstract& abs, std::string dir)
    : abs(abs), dir(dir)
{
    // pass
}

stat FileManager::reload() {
    list.clear();
    namespace fs = std::filesystem;

    try {
        for (const auto& entry : fs::directory_iterator(dir)) {
            list.push_back(entry);
        }
    } catch (const fs::filesystem_error& e) {
        log_err(e.what());
    }

    if (list.size() == 0) {
        pointer = 0;
    } else if (pointer >= list.size()) {
        pointer = list.size() - 1;
    }

    return stat::success;
}

stat FileManager::draw(dem x, dem y, dem w, dem h) {
    dem const min_w = 14;
    dem const min_h = 3;

    if (w < min_w || h < min_h) {
        log_err("bad sizes");
        return stat::error;
    }

    abs.draw_text(x, y, dir);
    for (std::size_t q = 0; q < list.size(); ++q) {
        abs.draw_text(3, q+1, list[q].path().filename());
    }
    return stat::success;

}