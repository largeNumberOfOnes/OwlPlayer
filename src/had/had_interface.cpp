#include "had_interface.h"
#include "had_keys.h"
#include "had_types.h"

#include <ncurses.h>
#include <locale.h>
#include <assert.h>



namespace had {
    Interface::Interface(const Logger& log) : log(log) {
        setlocale(LC_ALL, "");
        initscr();
        start_color();
        noecho();
        // halfdelay(1);
        timeout(0);
        cbreak();
        // raw();
        curs_set(0);
        keypad(stdscr, true);
        assert(can_change_color()); // DEV
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
        if (refresh() == OK){
            return Res::success;
        } else {
            return Res::error;
        }
    }

    Res Interface::cls() {
        // if (clear() == OK){
        if (erase() == OK){
            return Res::success;
        } else {
            return Res::error;
        }
    }

    static int color_count = 7;
    Res Interface::create_color(
        Color& col,
        int tr, int tg, int tb,
        int br, int bg, int bb
    ) {
        if (2*color_count + 1 >= COLORS && color_count >= COLOR_PAIRS) {
            log.log_err("Too many colors");
            return Res::error;
        }
        int ret1 = init_color(
            2 * color_count, tr*1000/256, tg*1000/256, tb*1000/256
        );
        int ret2 = init_color(
            2 * color_count + 1, br*1000/256, bg*1000/256, bb*1000/256
        );
        int ret3 = init_pair(
            color_count,
            2 * color_count,
            2 * color_count + 1
        );

        if (ret1 == OK && ret2 == OK && ret3 == OK) {
            col.color = color_count++;
            return Res::success;
        } else {
            return Res::error;
        }
    }
    Res Interface::create_text_color(
        Color& col,
        int tr, int tg, int tb
    ) {
        if (2*color_count + 1 >= COLORS && color_count >= COLOR_PAIRS) {
            log.log_err("Too many colors");
            return Res::error;
        }
        int ret1 = init_color(
            2 * color_count, tr*1000/256, tg*1000/256, tb*1000/256
        );
        int ret2 = init_pair(
            color_count,
            2 * color_count,
            COLOR_BLACK
        );

        if (ret1 == OK && ret2 == OK) {
            col.color = color_count++;
            return Res::success;
        } else {
            return Res::error;
        }
    }

    // If 'cb' is set to 'false' the the 'text color' will be changed,
    //                                       'background color' otherwise.
    Res Interface::change_color(Color& col, bool cb, int r, int g, int b) {
        int ret = init_color(
            2 * col.color + cb, r*1000/256, g*1000/256, b*1000/256
        );
        if (ret == OK) {
            return Res::success;
        } else {
            return Res::error;
        }
    }

    Res Interface::set_color(const Color& col) {
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

    KeySequence pre_catch_key_seq(int ch) {
        if ('a' <= ch && ch <= 'z') {
            return KeySequence{char_to_key(ch)};
        }
        if ('A' <= ch && ch <= 'Z') {
            return KeySequence{char_to_key(ch - 'A' + 'a')}.add_shift();
        }
        if ('1' <= ch && ch <= '0') {
            return KeySequence{char_to_key(ch)};
        }
        if (true) {
            switch (ch) {
                case ' '      : return KeySequence{Key::space};
                case KEY_UP   : return KeySequence{had::Key::arrow_up};
                case KEY_DOWN : return KeySequence{had::Key::arrow_down};
                case KEY_RIGHT: return KeySequence{had::Key::arrow_rigth};
                case KEY_LEFT : return KeySequence{had::Key::arrow_left};
                case KEY_ENTER: return KeySequence{had::Key::enter};
                case KEY_BACKSPACE: return KeySequence{had::Key::backspace};
            }
        }
        if (1 <= ch && ch <= 26) {
            return KeySequence{char_to_key('a' + ch - 1)}.add_ctrl();
        }
        return KeySequence::create_empty();
    }

    KeySequence Interface::catch_key_seq() {
        int ch = getch();
        // KeySequence seq;
        if (ch == 27) {
            return pre_catch_key_seq(getch()).add_alt();
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

    void Drawer::set(Dem x, Dem y, Dem w, Dem h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    Res Drawer::draw_symbol(Dem x, Dem y, char ch) {
        if (mvaddch(this->y + y, this->x + x, ch) == OK)
            return Res::success;
        else
            return Res::error;
    }

    Res Drawer::draw_wide_symbol(Dem x, Dem y, wchar_t ch) {
        cchar_t cch;
        wchar_t ch_arr[2] = {ch, '\0'};
        setcchar(&cch, ch_arr, WA_NORMAL, 0, NULL);
        if (mvadd_wch(this->y + y, this->x + x, &cch) == OK)
            return Res::success;
        else
            return Res::error;
    }

    Res Drawer::draw_sp_symbol(Dem x, Dem y, SpSymbol ch) {
        wchar_t ch_;
        switch (ch) {
            case SpSymbol::list_symbol        : ch_ = L'┣'; break;
            case SpSymbol::list_end_symbol    : ch_ = L'┗'; break;
            case SpSymbol::list_line_symbol   : ch_ = L'━'; break;
            case SpSymbol::scrol_up_symbol    : ch_ = L'▲'; break;
            case SpSymbol::scrol_middle_symbol: ch_ = L'║'; break;
            case SpSymbol::scrol_bottom_symbol: ch_ = L'▼'; break;

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

    Res Drawer::draw_text(Dem x, Dem y, std::string str) {
        if (mvprintw(this->y + y, this->x + x, "%s", str.c_str()) == OK)
            return Res::success;
        else
            return Res::error;
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
        for (int q = 0; q < w; ++q) {
            mvhline(y + q, x, ' ', w);
        } // DEV [Ret code]
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
