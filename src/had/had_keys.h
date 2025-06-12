

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
    };
    // enum struct KeyMod {
    //     shift,
    //     ctrl,
    //     alt,
    //     none,
    // };
    inline Key char_to_key(int num) { // Only for a, b, ..., z
        return static_cast<Key>(num - 'a');
    }
    // inline int key_to_keynum(Key key) { // Only for a, b, ..., z
    //     return 'a' + static_cast<int>(key);
    // }
    inline std::string key_to_str(Key key) {
        int num = static_cast<int>(key);
        if (0 <= num && num <= 25) {
            char ret[2];
            ret[0] = 'a' + num;
            ret[1] = '\0';
            return ret;
        }
        switch(key) {
            case Key::space: return "space";
            default:         return "unknown";
        }
    }
    // inline std::string keymod_to_str(KeyMod mod) {
    //     switch(mod) {
    //         case KeyMod::alt  : return "alt";
    //         case KeyMod::ctrl : return "ctrl";
    //         case KeyMod::shift: return "shift";
    //         default:         return "unknown";
    //     }
    // }
    
    class KeySequence {
        bool is_empty_val = false;
        Key key;
        // KeyMod mod;
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
    };

    // static std::unordered_map<std::string, Key> key_map {
    //     std::pair("q", Key::q)
    // };
};
