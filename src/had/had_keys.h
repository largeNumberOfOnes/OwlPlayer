

#pragma once

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
        backspace,
    };
    inline Key char_to_key(char ch) { // Only for a, b, ..., z
        return static_cast<Key>(ch - 'a');
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
            case Key::backspace  : return "backspace";
            case Key::arrow_up   : return "arrow_up";
            case Key::arrow_down : return "arrow_down";
            case Key::arrow_left : return "arrow_left";
            case Key::arrow_rigth: return "arrow_rigth";
            default:         return "unknown";
        }
    }
    
    class KeySequence {
        bool is_empty_val = false;
        Key key;
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

            std::string to_str() const {
                std::string str = key_to_str(key);
                if (is_ctrl)  { str = "ctrl "  + str; }
                if (is_shift) { str = "shift " + str; }
                if (is_alt)   { str = "alt "   + str; }
                return str;
            }

            static KeySequence create_empty() {
                return KeySequence{};
            }

            bool is_empty() const {
                return is_empty_val;
            }

            bool operator==(const KeySequence& other) const {
                return key      == other.key
                    && is_alt   == other.is_alt
                    && is_ctrl  == other.is_ctrl
                    && is_shift == other.is_shift
                    ||
                    is_empty_val && other.is_empty_val
                ;
            }
    };

    // static std::unordered_map<std::string, Key> key_map {
    //     std::pair("q", Key::q)
    // };
};
