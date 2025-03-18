#include "fileManager.h"
#include "intf.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstring>



FileManager::FileManager(std::string dir)
    : dir(dir)
{
    draw_line_buf = new char[draw_line_buf_size];
    if (!draw_line_buf)
        log_err("draw_line_buf is not allocated");
    for (had::dem q = 0; q < draw_line_buf_size; ++q)
        draw_line_buf[q] = ' ';
}

FileManager::~FileManager() {
    delete [] draw_line_buf;
}

res FileManager::up() {
    if (list_size == 0)
        return res::success;

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

    return res::success;
}

res FileManager::down() {
    if (list_size == 0)
        return res::success;

    if (pointer + 1 == list_size) {
        pointer = 0;
        top = 0;
    } else {
        ++pointer;
        if (pointer >= top + size)
            ++top;
    }

    return res::success;
}

res FileManager::reload() {
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

    return res::success;
}

res FileManager::resize(had::dem new_size) {
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

    return res::success;
}

bool FileManager::is_enougth_space(had::dem w, had::dem h) {
    return w >= min_w && h >= min_h;
}

res FileManager::draw(had::dem x, had::dem y, had::dem w, had::dem h) {
    if (w < min_w || h < min_h) {
        log_err("bad sizes");
        return res::error;
    }
    if (h != size + 2) {
        resize(h - 2);
    }

    for (had::dem q = 0; q < w; ++q)
        draw_line_buf[q] = ' ';
    for (std::size_t q = 0; q < h; ++q)
        had::draw_text(x, y + q, draw_line_buf);
        
    had::draw_text(x, y, dir.c_str());
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
            if (list[top+q].is_directory()) intf::setcol_dir_selected();
            else                            intf::setcol_file_selected();
            // draw_line_buf[1] = '>';
        } else {
            if (list[top+q].is_directory()) intf::setcol_dir();
            else                            intf::setcol_file();
            // draw_line_buf[1] = ' ';
        }
        had::draw_text(x, y + q + 1, draw_line_buf);
        if (top + q + 1 == list_size) intf::draw_list_end_symbol(x + 1, y + q + 1);
        else                          intf::draw_list_symbol(x + 1, y + q + 1);
        intf::setcol_default();
    }

    had::draw_text(x, y+h-1, ":");

    return res::success;
}
