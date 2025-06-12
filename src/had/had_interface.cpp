#include "had_interface.h"
#include "had_keys.h"
#include "had_types.h"

#include <curses.h>
#include <locale.h>


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
        if (clear() == OK){
            return Res::success;
        } else {
            return Res::error;
        }
    }

    static int color_count = 1;
    Res Interface::create_color(
        Color col,
        int tr, int tg, int tb,
        int br, int bg, int bb
    ) {
        if (2*color_count + 1 >= COLORS && color_count >= COLOR_PAIRS) {
            log.log_err("Too many colors");
            return Res::error;
        }
        int ret1 = init_color(
            2 * color_count + 1, tr*1000/256, tg*1000/256, tb*1000/256
        );
        int ret2 = init_color(
            2 * color_count, br*1000/256, bg*1000/256, bb*1000/256
        );
        int ret3 = init_pair(
            color_count,
            2 * color_count,
            2 * color_count + 2
        );

        if (ret1 == OK && ret2 == OK && ret3 == OK) {
            col.color = color_count++;
            return Res::success;
        } else {
            return Res::error;
        }
    }

    // If 'cb' is set to 'true' the the 'text color' will be changed,
    //                                       'background color' otherwise.
    Res Interface::change_color(Color col, bool cb, int r, int g, int b) {
        int ret = init_color(
            2 * col.color + cb, r*1000/256, g*1000/256, b*1000/256
        );
        if (ret == OK) {
            return Res::success;
        } else {
            return Res::error;
        }
    }

    Res Interface::set_color(Color col) {
        int ret = attron(COLOR_PAIR(col.color)) == OK;
        if (ret == OK) {
            return Res::success;
        } else {
            return Res::error;
        }
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
        if (ch == ' ') {
            return KeySequence{Key::space};
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
            case SpSymbol::list_symbol    : ch_ = L'├'; break;
            case SpSymbol::list_end_symbol: ch_ = L'└'; break;
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
        // std::string slider(len, '-');
        // slider[val] = '0';
        // mvprintw(this->y + y, this->x + x, "%s", slider.c_str());
        for (int q = 0; q < len; ++q) {
            if (q == val) {
                draw_wide_symbol(x + q, y, L'■');
            } else {
                draw_wide_symbol(x + q, y, L'─');
            }
        }
        return Res::success;
    }

    Res Drawer::set_color(Color col) {
        return interface.set_color(col);
    }

    Dem Drawer::get_width() {
        return w;
    }

    Dem Drawer::get_heigth() {
        return h;
    }

    void Drawer::set_width(Dem w) {
        this->w = w;
    }

    void Drawer::set_heigth(Dem h) {
        this->h = h;
    }
}
