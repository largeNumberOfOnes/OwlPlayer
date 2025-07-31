/**
 * pass
 */

#pragma once

#include "unicodeString.h"

#include <string_view>
#include <string>



namespace utils {
    class StringEditor {
        public:
            using CursorPos = int;

        private:
            UnicodeString str{""};
            CursorPos pos = 0;

        public:
            void set(std::string_view str, CursorPos pos);
            std::string_view get_string();
            CursorPos get_cursor();

            void move_cursor(CursorPos pos);
            void move_cursor_right();
            void move_cursor_left();
            void move_on_word_right();
            void move_on_word_left();
            void insert_symbol(char32_t symbol);
            void delete_symbol_back();
            void delete_symbol_front();
    };
}
