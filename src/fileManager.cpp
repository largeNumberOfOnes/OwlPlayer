#include "fileManager.h"

#include "had/had.h"

#include <algorithm>

#include <cstddef>
#include <cstdio>
#include <cstring>



FileManager::FileManager(
    std::string dir, 
    std::function<void(std::string)> call_on_file,
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
    draw_line_buf = new char[draw_line_buf_size];
    if (!draw_line_buf)
        log.log_err("draw_line_buf is not allocated");
    for (had::Dem q = 0; q < draw_line_buf_size; ++q)
        draw_line_buf[q] = ' ';

    if (!this->dir.ends_with('/')) {
        this->dir += '/';
    }
    reload();    
}

FileManager::~FileManager() {
    delete [] draw_line_buf;
}

had::Res FileManager::go() {
    if (list[pointer].is_directory()) {
        std::cout << list[pointer].path() << std::endl;
        dir = list[pointer].path();
        dir += '/';
        reload();
    } else if (list[pointer].is_regular_file()) {
        std::string path = list[pointer].path();
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

had::Res FileManager::reload() {
    list.clear();
    namespace fs = std::filesystem;

    // DEV [maybe there should be placed try-catch block]
    for (const auto& entry : fs::directory_iterator(dir)) {
        list.push_back(entry);
    }

    std::sort(
        list.begin(), list.end(),
        [](auto a, auto b) {
            if (a.is_directory() and b.is_directory()
                || a.is_regular_file() and b.is_regular_file()
            ) {
                return a.path() < b.path();
            } else if (a.is_directory()) {
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

had::Res FileManager::resize() {
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
    if (!drawer.cls() && !draw()) {
        return had::Res::success;
    } else {
        return had::Res::error;
    }
}

bool FileManager::is_enougth_space(had::Dem w, had::Dem h) {
    return w >= min_w && h >= min_h;
}

had::Res FileManager::draw() {
    had::Dem w = drawer.get_width();
    had::Dem h = drawer.get_height();
    if (w < min_w || h < min_h) {
        log.log_err("bad sizes");
        return had::Res::error;
    }

    for (had::Dem q = 0; q < w; ++q)
        draw_line_buf[q] = ' ';
    for (std::size_t q = 0; q < h; ++q)
        drawer.draw_text(0, q, draw_line_buf);
        
    drawer.draw_text(0, 0, dir.c_str());
    std::size_t files_count = (size > list_size) ? list_size : size;

    for (std::size_t q = 0; q < files_count; ++q) {
        std::strncpy(
            draw_line_buf + line_offset,
            list[top+q].path().filename().c_str(),
            draw_line_buf_size - line_offset
        );

        std::size_t str_len = std::strlen(draw_line_buf + line_offset);

        if (str_len + free_space > w)
            std::strcpy(draw_line_buf + w - free_space, "...  ");

        // draw_line_buf[1] = (top + q == pointer) ? '>' : ' ';

        if (top + q == pointer) {
            // if (list[top+q].is_directory()) intf::setcol_dir_selected();
            if (list[top+q].is_directory()) {
                drawer.set_color(setup.colors.dir_selected);
            } else {
                drawer.set_color(setup.colors.file_selected);
            }
            // draw_line_buf[1] = '>';
        } else {
            if (list[top+q].is_directory()) {
                drawer.set_color(setup.colors.dir);
            } else {
                drawer.set_color(setup.colors.file);
            }
            // draw_line_buf[1] = ' ';
        }
        drawer.draw_text(0, q + 1, draw_line_buf);
        if (top + q + 1 == list_size) {
            drawer.draw_sp_symbol(1, q + 1, had::SpSymbol::list_end_symbol);
        } else {
            drawer.draw_sp_symbol(1, q + 1, had::SpSymbol::list_symbol);
        }
        drawer.draw_sp_symbol(2, q + 1, had::SpSymbol::list_line_symbol);
        drawer.set_color(setup.colors.def);
    }

    drawer.draw_symbol(0, h-1, ':');

    return had::Res::success;
}
