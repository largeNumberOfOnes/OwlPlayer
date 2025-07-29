#include "had_interface.h"

#include "had_keys.h"
#include "had_types.h"

#include <ncurses.h>

#include <string_view>
#include <optional>
#include <string>



namespace had {
    Interface::Interface(const Logger &log) : log(log) {
        setlocale(LC_ALL, "");
        initscr();
        start_color();
        noecho();
        // halfdelay(1);
        timeout(0);
        // nodelay(stdscr, true);
        // cbreak();
        meta(stdscr, true);
        raw();
        nonl();
        curs_set(0);
        keypad(stdscr, true);
        // assert(can_change_color()); // DEV
    }

    Interface::~Interface() {
        endwin();
    }

    Dem Interface::get_width() {
        int w = getmaxx(stdscr);
        return (w == ERR) ? 0 : w;
    }

    Dem Interface::get_height() {
        int h = getmaxy(stdscr);
        return (h == ERR) ? 0 : h;
    }

    Res Interface::update() {
        if (refresh() == OK) {
            return Res::success;
        } else {
            return Res::error;
        }
    }

    Res Interface::cls() {
        // if (clear() == OK){
        if (erase() == OK) {
            return Res::success;
        } else {
            return Res::error;
        }
    }

static int color_count = 7;
Res Interface::create_color(Color &col, int tr, int tg, int tb, int br, int bg,
                            int bb) {
  if (2 * color_count + 1 >= COLORS && color_count >= COLOR_PAIRS) {
    log.log_err("Too many colors");
    return Res::error;
  }
  int ret1 = init_color(2 * color_count, tr * 1000 / 256, tg * 1000 / 256,
                        tb * 1000 / 256);
  int ret2 = init_color(2 * color_count + 1, br * 1000 / 256, bg * 1000 / 256,
                        bb * 1000 / 256);
  int ret3 = init_pair(color_count, 2 * color_count, 2 * color_count + 1);

  if (ret1 == OK && ret2 == OK && ret3 == OK) {
    col.color = color_count++;
    return Res::success;
  } else {
    return Res::error;
  }
}
Res Interface::create_text_color(Color &col, int tr, int tg, int tb) {
  if (2 * color_count + 1 >= COLORS && color_count >= COLOR_PAIRS) {
    log.log_err("Too many colors");
    return Res::error;
  }
  int ret1 = init_color(2 * color_count, tr * 1000 / 256, tg * 1000 / 256,
                        tb * 1000 / 256);
  int ret2 = init_pair(color_count, 2 * color_count, COLOR_BLACK);

  if (ret1 == OK && ret2 == OK) {
    col.color = color_count++;
    return Res::success;
  } else {
    return Res::error;
  }
}

// If 'cb' is set to 'false' the 'text color' will be changed,
//                                       'background color' otherwise.
Res Interface::change_color(Color &col, bool cb, int r, int g, int b) {
  int ret = init_color(2 * col.color + cb, r * 1000 / 256, g * 1000 / 256,
                       b * 1000 / 256);
  if (ret == OK) {
    return Res::success;
  } else {
    return Res::error;
  }
}

Res Interface::set_color(const Color &col) {
  int ret = ERR;
  if (col.color == 0) {
    ret = attroff(~0);
  } else {
    ret = attron(COLOR_PAIR(col.color));
  }
  if (ret == OK) {
    return Res::success;
  } else {
    return Res::error;
  }
}

    Res Interface::get_default_color(Color& col) {
        col.color = 0;
        return Res::success;
    }

    std::optional<KeySequence> try_int_to_cyrillic_lower(char32_t ch) {
        switch (ch) {
            case 0xb0d0: return KeySequence(Key::r1 ); // а
            case 0xb1d0: return KeySequence(Key::r2 ); // б
            case 0xb2d0: return KeySequence(Key::r3 ); // в
            case 0xb3d0: return KeySequence(Key::r4 ); // г
            case 0xb4d0: return KeySequence(Key::r5 ); // д
            case 0xb5d0: return KeySequence(Key::r6 ); // е
            case 0x91d1: return KeySequence(Key::r7 ); // ё
            case 0xb6d0: return KeySequence(Key::r8 ); // ж
            case 0xb7d0: return KeySequence(Key::r9 ); // з
            case 0xb8d0: return KeySequence(Key::r10); // и
            case 0xb9d0: return KeySequence(Key::r11); // й
            case 0xbad0: return KeySequence(Key::r12); // к
            case 0xbbd0: return KeySequence(Key::r13); // л
            case 0xbcd0: return KeySequence(Key::r14); // м
            case 0xbdd0: return KeySequence(Key::r15); // н
            case 0xbed0: return KeySequence(Key::r16); // о
            case 0xbfd0: return KeySequence(Key::r17); // п
            case 0x80d1: return KeySequence(Key::r18); // р
            case 0x81d1: return KeySequence(Key::r19); // с
            case 0x82d1: return KeySequence(Key::r20); // т
            case 0x83d1: return KeySequence(Key::r21); // у
            case 0x84d1: return KeySequence(Key::r22); // ф
            case 0x85d1: return KeySequence(Key::r23); // х
            case 0x86d1: return KeySequence(Key::r24); // ц
            case 0x87d1: return KeySequence(Key::r25); // ч
            case 0x88d1: return KeySequence(Key::r26); // ш
            case 0x89d1: return KeySequence(Key::r27); // щ
            case 0x8ad1: return KeySequence(Key::r28); // ъ
            case 0x8bd1: return KeySequence(Key::r29); // ы
            case 0x8cd1: return KeySequence(Key::r30); // ь
            case 0x8dd1: return KeySequence(Key::r31); // э
            case 0x8ed1: return KeySequence(Key::r32); // ю
            case 0x8fd1: return KeySequence(Key::r33); // я
            default: return std::nullopt;
        };
    }

    static std::optional<KeySequence> try_int_to_cyrillic_upper(
                                                            char32_t ch) {
        switch(ch) {
            case 0x90d0: return KeySequence(Key::r1 ).add_shift(); // А
            case 0x91d0: return KeySequence(Key::r2 ).add_shift(); // Б
            case 0x92d0: return KeySequence(Key::r3 ).add_shift(); // В
            case 0x93d0: return KeySequence(Key::r4 ).add_shift(); // Г
            case 0x94d0: return KeySequence(Key::r5 ).add_shift(); // Д
            case 0x95d0: return KeySequence(Key::r6 ).add_shift(); // Е
            case 0x81d0: return KeySequence(Key::r7 ).add_shift(); // Ё
            case 0x96d0: return KeySequence(Key::r8 ).add_shift(); // Ж
            case 0x97d0: return KeySequence(Key::r9 ).add_shift(); // З
            case 0x98d0: return KeySequence(Key::r10).add_shift(); // И
            case 0x99d0: return KeySequence(Key::r11).add_shift(); // Й
            case 0x9ad0: return KeySequence(Key::r12).add_shift(); // К
            case 0x9bd0: return KeySequence(Key::r13).add_shift(); // Л
            case 0x9cd0: return KeySequence(Key::r14).add_shift(); // М
            case 0x9dd0: return KeySequence(Key::r15).add_shift(); // Н
            case 0x9ed0: return KeySequence(Key::r16).add_shift(); // О
            case 0x9fd0: return KeySequence(Key::r17).add_shift(); // П
            case 0xa0d0: return KeySequence(Key::r18).add_shift(); // Р
            case 0xa1d0: return KeySequence(Key::r19).add_shift(); // С
            case 0xa2d0: return KeySequence(Key::r20).add_shift(); // Т
            case 0xa3d0: return KeySequence(Key::r21).add_shift(); // У
            case 0xa4d0: return KeySequence(Key::r22).add_shift(); // Ф
            case 0xa5d0: return KeySequence(Key::r23).add_shift(); // Х
            case 0xa6d0: return KeySequence(Key::r24).add_shift(); // Ц
            case 0xa7d0: return KeySequence(Key::r25).add_shift(); // Ч
            case 0xa8d0: return KeySequence(Key::r26).add_shift(); // Ш
            case 0xa9d0: return KeySequence(Key::r27).add_shift(); // Щ
            case 0xaad0: return KeySequence(Key::r28).add_shift(); // Ъ
            case 0xabd0: return KeySequence(Key::r29).add_shift(); // Ы
            case 0xacd0: return KeySequence(Key::r30).add_shift(); // Ь
            case 0xadd0: return KeySequence(Key::r31).add_shift(); // Э
            case 0xaed0: return KeySequence(Key::r32).add_shift(); // Ю
            case 0xafd0: return KeySequence(Key::r33).add_shift(); // Я
            default: return std::nullopt;
    };
    }

    std::optional<char32_t> try_cyrillic_to_int_lower(Key key) {
        switch (key) {
            case Key::r1 : return 0xb0d0; // а
            case Key::r2 : return 0xb1d0; // б
            case Key::r3 : return 0xb2d0; // в
            case Key::r4 : return 0xb3d0; // г
            case Key::r5 : return 0xb4d0; // д
            case Key::r6 : return 0xb5d0; // е
            case Key::r7 : return 0x91d1; // ё
            case Key::r8 : return 0xb6d0; // ж
            case Key::r9 : return 0xb7d0; // з
            case Key::r10: return 0xb8d0; // и
            case Key::r11: return 0xb9d0; // й
            case Key::r12: return 0xbad0; // к
            case Key::r13: return 0xbbd0; // л
            case Key::r14: return 0xbcd0; // м
            case Key::r15: return 0xbdd0; // н
            case Key::r16: return 0xbed0; // о
            case Key::r17: return 0xbfd0; // п
            case Key::r18: return 0x80d1; // р
            case Key::r19: return 0x81d1; // с
            case Key::r20: return 0x82d1; // т
            case Key::r21: return 0x83d1; // у
            case Key::r22: return 0x84d1; // ф
            case Key::r23: return 0x85d1; // х
            case Key::r24: return 0x86d1; // ц
            case Key::r25: return 0x87d1; // ч
            case Key::r26: return 0x88d1; // ш
            case Key::r27: return 0x89d1; // щ
            case Key::r28: return 0x8ad1; // ъ
            case Key::r29: return 0x8bd1; // ы
            case Key::r30: return 0x8cd1; // ь
            case Key::r31: return 0x8dd1; // э
            case Key::r32: return 0x8ed1; // ю
            case Key::r33: return 0x8fd1; // я
            default: return std::nullopt;
        };
    }

    static std::optional<char32_t> try_cyrillic_to_int_upper(Key key) {
        switch (key) {
            case Key::r1 : return 0x90d0; // А
            case Key::r2 : return 0x91d0; // Б
            case Key::r3 : return 0x92d0; // В
            case Key::r4 : return 0x93d0; // Г
            case Key::r5 : return 0x94d0; // Д
            case Key::r6 : return 0x95d0; // Е
            case Key::r7 : return 0x81d0; // Ё
            case Key::r8 : return 0x96d0; // Ж
            case Key::r9 : return 0x97d0; // З
            case Key::r10: return 0x98d0; // И
            case Key::r11: return 0x99d0; // Й
            case Key::r12: return 0x9ad0; // К
            case Key::r13: return 0x9bd0; // Л
            case Key::r14: return 0x9cd0; // М
            case Key::r15: return 0x9dd0; // Н
            case Key::r16: return 0x9ed0; // О
            case Key::r17: return 0x9fd0; // П
            case Key::r18: return 0xa0d0; // Р
            case Key::r19: return 0xa1d0; // С
            case Key::r20: return 0xa2d0; // Т
            case Key::r21: return 0xa3d0; // У
            case Key::r22: return 0xa4d0; // Ф
            case Key::r23: return 0xa5d0; // Х
            case Key::r24: return 0xa6d0; // Ц
            case Key::r25: return 0xa7d0; // Ч
            case Key::r26: return 0xa8d0; // Ш
            case Key::r27: return 0xa9d0; // Щ
            case Key::r28: return 0xaad0; // Ъ
            case Key::r29: return 0xabd0; // Ы
            case Key::r30: return 0xacd0; // Ь
            case Key::r31: return 0xadd0; // Э
            case Key::r32: return 0xaed0; // Ю
            case Key::r33: return 0xafd0; // Я
            default: return std::nullopt;
        };
    }

    static std::optional<KeySequence> try_int_to_cyrillic(char32_t ch) {
        auto res = try_int_to_cyrillic_lower(ch);
        if (res.has_value()) {
            return res;
        } else {
            return try_int_to_cyrillic_upper(ch);
        }
    }

    static std::optional<char32_t> try_cyrillic_to_int(KeySequence seq) {
        if (seq.has_shift()) {
            return try_cyrillic_to_int_upper(seq.get_key());
        } else {
            return try_cyrillic_to_int_lower(seq.get_key());
        }
    }

    static std::optional<KeySequence> try_catch_letter(char32_t ch) {
        if ('a' <= ch && ch <= 'z') {
            return KeySequence{char_to_key(ch)};
        }
        if ('A' <= ch && ch <= 'Z') {
            return KeySequence{char_to_key(ch - 'A' + 'a')}.add_shift();
        }
        if ('1' <= ch && ch <= '0') {
            return KeySequence{char_to_key(ch)};
        }
        return try_int_to_cyrillic(ch);
    }

    static KeySequence pre_catch_key_seq(char32_t ch) {
        if (true) {
            switch (ch) {
                case ' '      : return KeySequence{Key::space};
                case KEY_UP   : return KeySequence{had::Key::arrow_up};
                case KEY_DOWN : return KeySequence{had::Key::arrow_down};
                case KEY_RIGHT: return KeySequence{had::Key::arrow_rigth};
                case KEY_LEFT : return KeySequence{had::Key::arrow_left};
                case 13       : return KeySequence{had::Key::enter};
                case KEY_BACKSPACE:
                                return KeySequence{had::Key::backspace};
            }
        }
        if (ch == 27) { // Esc key code
            return KeySequence{had::Key::esc};
        }
        // if (1 <= ch && ch <= 26) {
        //     return KeySequence{char_to_key('a' + ch - 1)}.add_ctrl();
        // }
        auto seq = try_catch_letter(ch);
        if (seq.has_value()) {
            return seq.value();
        }
        return KeySequence::create_empty();
    }

    KeySequence Interface::catch_key_seq() {
        auto is_escape_seq = +[](int ch) { return (ch == 27); };
        auto is_1b_uni_seq = +[](int ch) { return ((ch & 0x80) == 0x00); };
        auto is_2b_uni_seq = +[](int ch) { return ((ch & 0xE0) == 0xC0); };
        auto is_3b_uni_seq = +[](int ch) { return ((ch & 0xF0) == 0xE0); };
        auto is_4b_uni_seq = +[](int ch) { return ((ch & 0xF8) == 0xF0); };

        char32_t ch = getch();
        char *arr = reinterpret_cast<char *>(&ch);
        if (ch == -1) {
            return KeySequence::create_empty();
        };
        if (is_escape_seq(ch)) {
            int ch = getch();
            arr[1] = ch;
        } else if (is_1b_uni_seq(ch)) {
            // Nothing needs to be done
        } else if (is_2b_uni_seq(ch)) {
            arr[1] = getch();
        } else if (is_3b_uni_seq(ch)) {
            arr[1] = getch();
            arr[2] = getch();
        } else if (is_4b_uni_seq(ch)) {
            arr[1] = getch();
            arr[2] = getch();
            arr[3] = getch();
        }

        constexpr bool LOG_INPUT = false;
        if constexpr (LOG_INPUT) {
            if (ch != -1) {
                std::string str = std::to_string(arr[0] & 0xFF) + " "
                                + std::to_string(arr[1] & 0xFF) + " "
                                + std::to_string(arr[2] & 0xFF) + " "
                                + std::to_string(arr[3] & 0xFF) + " ";
                log.log_info(str);
            }
        }

        return pre_catch_key_seq(ch);
    }

    Drawer Interface::produce_drawer() {
        return Drawer{*this, 0, 0, get_width(), get_height(), log};
    }

    bool Interface::is_resized() {
        if (old_w == get_width() && old_h == get_height()) {
            return false;
        } else {
            old_w = get_width();
            old_h = get_height();
            return true;
        }
    }
}

namespace had {
    Drawer::Drawer(
        Interface& interface,
        Dem x, Dem y, Dem w, Dem h,
        const had::Logger& log
    )
        : interface(interface)
        , x(x), y(y), w(w), h(h)
        , log(log)
    {}

    Drawer::Drawer(
        Interface& interface,
        const had::Logger& log
    )
        : interface(interface)
        , x(0), y(0), w(0), h(0)
        , log(log)
    {}

    void Drawer::set(Dem x, Dem y, Dem w, Dem h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    Res Drawer::draw_symbol(Dem x, Dem y, char ch) {
        if (mvaddch(this->y + y, this->x + x, ch) == OK) {
            return Res::success;
        } else {
            return Res::error;
        }
    }

    Res Drawer::draw_wide_symbol(Dem x, Dem y, wchar_t ch) {
        cchar_t cch;
        wchar_t ch_arr[2] = {ch, '\0'};
        setcchar(&cch, ch_arr, WA_NORMAL, 0, NULL);
        if (mvadd_wch(this->y + y, this->x + x, &cch) == OK) {
            return Res::success;
        } else {
            return Res::error;
        }
    }

    Res Drawer::draw_sp_symbol(Dem x, Dem y, SpSymbol ch) {
        wchar_t ch_;
        switch (ch) {
            case SpSymbol::list_symbol:         ch_ = L'┣'; break;
            case SpSymbol::list_end_symbol:     ch_ = L'┗'; break;
            case SpSymbol::list_line_symbol:    ch_ = L'━'; break;
            case SpSymbol::scrol_up_symbol:     ch_ = L'▲'; break;
            case SpSymbol::scrol_middle_symbol: ch_ = L'║'; break;
            case SpSymbol::scrol_bottom_symbol: ch_ = L'▼'; break;
            case SpSymbol::back_arrow_symbol:   ch_ = L'↪'; break;

            case SpSymbol::spectre_0_symbol: ch_ = L' '; break;
            case SpSymbol::spectre_1_symbol: ch_ = L'▁'; break;
            case SpSymbol::spectre_2_symbol: ch_ = L'▂'; break;
            case SpSymbol::spectre_3_symbol: ch_ = L'▃'; break;
            case SpSymbol::spectre_4_symbol: ch_ = L'▄'; break;
            case SpSymbol::spectre_5_symbol: ch_ = L'▆'; break;
            case SpSymbol::spectre_6_symbol: ch_ = L'▆'; break;
            case SpSymbol::spectre_7_symbol: ch_ = L'▇'; break;
            case SpSymbol::spectre_8_symbol: ch_ = L'█'; break;
        }
        return draw_wide_symbol(x, y, ch_);
    }

    Res Drawer::draw_text(Dem x, Dem y, std::string_view str) {
        if (mvaddstr(this->y + y, this->x + x, str.data()) == OK) {
            return Res::success;
        } else {
            return Res::error;
        }
    }

    Res Drawer::draw_text_n(Dem x, Dem y, std::string_view str, int n) {
        if (mvaddnstr(this->y + y, this->x + x, str.data(), n) == OK) {
            return Res::success;
        } else {
            return Res::error;
        }
    }

    Res Drawer::draw_slider(Dem x, Dem y, Dem len, Dem val) {
        for (int q = 0; q < len; ++q) {
            if (q == val) {
                draw_wide_symbol(x + q, y, L'■');
            } else {
                draw_wide_symbol(x + q, y, L'─');
            }
        }
        return Res::success;
    }

    Res Drawer::cls() {
        for (int q = 0; q < h; ++q) {
            // for (int e = 0; e < w; ++e) {
            //     draw_symbol(x + e, y + q, ' ');
            // }
            int ret = mvhline(y + q, x, ' ', w);
            if (ret == ERR) {
                return Res::error;
            }
        }
        return Res::success;
    }

    Res Drawer::set_color(const Color& col) {
        return interface.set_color(col);
    }

    Dem Drawer::get_width() {
        return w;
    }

    Dem Drawer::get_height() {
        return h;
    }

    void Drawer::set_width(Dem w) {
        this->w = w;
    }

    void Drawer::set_heigth(Dem h) {
        this->h = h;
    }
}
