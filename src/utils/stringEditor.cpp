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

    void StringEditor::move_on_word_right() {
        str.rfind_from(pos, ' ');
        // ++pos;
    }
}
