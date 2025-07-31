#include "stringEditor.h"

#include <chrono>
#include <iostream>
#include <string_view>
#include <thread>

#include "../had/had_interface.h"
#include "../had/had_keys.h"



int draw(utils::StringEditor& editor, had::Interface& interface,
                                                    had::Drawer& drawer) {
    drawer.cls();
    drawer.draw_text(0, 0, editor.get_string());
    drawer.draw_symbol(editor.get_cursor(), 1, '^');

    had::KeySequence seq = interface.catch_key_seq();
    if (seq == had::KeySequence{had::Key::esc}) {
        return 1;
    } else if (seq == had::KeySequence{had::Key::arrow_rigth}) {
        editor.move_cursor_right();
    } else if (seq == had::KeySequence{had::Key::arrow_left}) {
        editor.move_cursor_left();
    } else if (seq == had::KeySequence{had::Key::arrow_rigth}.add_ctrl()) {
        editor.move_on_word_right();
    } else if (seq == had::KeySequence{had::Key::arrow_left}.add_ctrl()) {
        editor.move_on_word_left();
    } else if (seq == had::KeySequence{had::Key::backspace}) {
        editor.delete_symbol_back();
    } else if (seq == had::KeySequence{had::Key::del}) {
        editor.delete_symbol_front();
    } else if (seq.is_alpha()) {
        editor.insert_symbol(seq.to_char());
    }

    return 0;
}

int main() {

    utils::StringEditor editor;
    std::string_view test_str{
        "The quick brown fox jumps over the lazy dog"
    };
    editor.set(test_str, 7);

    had::Interface interface{had::black_logger};
    had::Drawer drawer = interface.produce_drawer();

    while (true) {
        if (draw(editor, interface, drawer)) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    return 0;
}
