#include "fileManager.h"

#include "had/had.h"
#include "had/had_types.h"

#include <algorithm>

#include <cstddef>
#include <cstdio>
#include <cstring>



FileManager::FileManager(
    std::string dir,
    had::Drawer& drawer,
    const Setup& setup,
    const had::Logger& log
)
    : dir(dir)
    , drawer(drawer)
    , setup(setup)
    , log(log)
{
    draw_line_buf = new char[draw_line_buf_size];
    if (!draw_line_buf)
        log.log_err("draw_line_buf is not allocated");
    for (had::Dem q = 0; q < draw_line_buf_size; ++q)
        draw_line_buf[q] = ' ';
}

FileManager::~FileManager() {
    delete [] draw_line_buf;
}

had::Res FileManager::go() {
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

had::Res FileManager::resize(had::Dem new_size) {
    size = new_size;
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

bool FileManager::is_enougth_space(had::Dem w, had::Dem h) {
    return w >= min_w && h >= min_h;
}

had::Res FileManager::draw() {
    had::Dem w = drawer.get_width();
    had::Dem h = drawer.get_heigth();
    if (w < min_w || h < min_h) {
        log.log_err("bad sizes");
        return had::Res::error;
    }
    if (h != size + 2) {
        resize(h - 2);
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
        drawer.set_color(setup.colors.def);
    }

    drawer.draw_symbol(0, h-1, ':');

    return had::Res::success;
}
