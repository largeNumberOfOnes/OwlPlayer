/**
 * This module defines the keys and key combinations entered by the user,
 *    as well as the functions for interacting with them.
 */

#pragma once

#include <optional>
#include <unordered_map>
#include <string>
#include <vector>

namespace had {
    enum struct Key {
        a, b, c, d, e, f, g, h, i, j, k, l, m,
        n, o, p, q, r, s, t, u, v, w, x, y, z,
        arrow_rigth,
        arrow_left,
        arrow_up,
        arrow_down,
        space,
        enter,
        esc,
        backspace,
        del,
         r1,  r2,  r3,  r4,  t4,  r5,  r6,  r7,  r8,  r9, r10,
        r11, r12, r13, r14, t14, r15, r16, r17, r18, r19, r20,
        r21, r22, r23, r24, t24, r25, r26, r27, r28, r29, r30,
        r31, r32, r33, // Cyrillic characters in alphabetical order
    };

    inline Key char_to_key(char ch) { // Only for a, b, ..., z
        return static_cast<Key>(ch - 'a');
    }

    inline char key_to_char(Key key) { // Only for a, b, ..., z
        return 'a' + static_cast<char>(key);
    }

    inline std::string key_to_str(Key key) {
        int num = static_cast<int>(key);
        if (0 <= num && num <= 25) {
            char ret[2];
            ret[0] = 'a' + num;
            ret[1] = '\0';
            return ret;
        }
        switch(key) {
            case Key::space      : return "space";
            case Key::enter      : return "enter";
            case Key::esc        : return "esc";
            case Key::backspace  : return "backspace";
            case Key::del        : return "del";
            case Key::arrow_up   : return "arrow_up";
            case Key::arrow_down : return "arrow_down";
            case Key::arrow_left : return "arrow_left";
            case Key::arrow_rigth: return "arrow_rigth";
            default              : return "unknown";
        }
    }
    
    class KeySequence {
        bool is_empty_val = false;
        Key key = Key::a;
        bool is_ctrl  = false;
        bool is_shift = false;
        bool is_alt   = false;
        KeySequence() {
            is_empty_val = true;
        }
        
        public:
            KeySequence(Key key) {
                this->key = key;
            }

            KeySequence add_ctrl() {
                is_ctrl = true;
                return *this;
            }
            KeySequence add_shift() {
                is_shift = true;
                return *this;
            }
            KeySequence add_alt() {
                is_alt = true;
                return *this;
            }

            bool has_shift() const {
                return is_shift;
            }

            char to_char() const {
                char c = key_to_char(key);
                if (is_shift) {
                    return c - 'a' + 'A';
                }
                return c;
            }

            std::string to_str() const {
                if (is_empty()) {
                    return "";
                }
                std::string str = key_to_str(key);
                if (is_ctrl)  { str = "ctrl "  + str; }
                if (is_shift) { str = "shift " + str; }
                if (is_alt)   { str = "alt "   + str; }
                return str;
            }

            bool is_alpha() const {
                if (is_empty()) {
                    return false;
                }
                if (is_ctrl == true || is_alt == true) {
                    return false;
                }
                if ('a' <= key_to_char(key) && key_to_char(key) <= 'z') {
                    return true;
                }
                return false;
            }

            static KeySequence create_empty() {
                return KeySequence{};
            }

            bool is_empty() const {
                return is_empty_val;
            }

            Key get_key() const {
                return key;
            }

            bool operator==(const KeySequence& other) const {
                return key      == other.key
                    && is_alt   == other.is_alt
                    && is_ctrl  == other.is_ctrl
                    && is_shift == other.is_shift
                    && !is_empty_val
                    && !other.is_empty_val
                    ||
                    is_empty_val && other.is_empty_val
                ;
            }
    };

    std::optional<KeySequence> try_int_to_cyrillic_lower(char32_t ch);
    std::optional<char32_t> try_cyrillic_to_int_lower(Key key);
};
