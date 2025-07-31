#include "stringEditor.h"
#include <exception>
#include <string_view>



namespace utils {
    void StringEditor::set(std::string_view str, int pos) {
        this->str = str;
        this->pos = pos;
    }

    std::string_view StringEditor::get_string() {
        return str.to_string_view();
    }

    int StringEditor::get_cursor() {
        return pos;
    }

    void StringEditor::move_cursor(CursorPos pos) {
        pos = std::max(0, pos);
        pos = std::min<CursorPos>(pos, str.get_char_len());
        this->pos = pos;
    }

    void StringEditor::move_cursor_right() {
        move_cursor(pos + 1);
    }

    void StringEditor::move_cursor_left() {
        move_cursor(pos - 1);
    }

    void StringEditor::insert_symbol(char32_t symbol) {
        str.insert_symbol_by_char_pos(pos, symbol);
        ++pos;
    }

    void StringEditor::delete_symbol_back() {
        if (pos == 0) {
            return;
        }
        str.delete_symbol_by_char_pos(pos - 1);
        --pos;
    }

    void StringEditor::delete_symbol_front() {
        if (pos == str.get_char_len()) {
            return;
        }
        str.delete_symbol_by_char_pos(pos);
    }

    void StringEditor::move_on_word_right() {
        if (pos == str.get_char_len()) {
            return;
        }
        while (str.get_char(pos) == ' ') {
            ++pos;
        }
        std::optional<std::size_t> res = str.rfind_from(' ', pos);
        if (res.has_value()) {
            pos = res.value();
        } else {
            pos = str.get_char_len();
        }
    }

    void StringEditor::move_on_word_left() {
        while (pos > 0 && str.get_char(pos - 1) == ' ') {
            --pos;
        }
        std::optional<std::size_t> res = str.lfind_from(' ', pos);
        if (res.has_value()) {
            pos = res.value() + 1;
        } else {
            pos = 0;
        }
    }
}
