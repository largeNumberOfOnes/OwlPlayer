/**
 * pass
 */

#pragma once

#include <string_view>
#include <string>



class StringEditor {
    std::string str;
    int pos = 0;

    public:
        void set(std::string_view str, int curs_pos);
        std::string_view get_string();
        int get_cursor();
        std::string drop();
};
