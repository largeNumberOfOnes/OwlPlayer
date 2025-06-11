#include "had_interface.h"

#include <curses.h>
#include <locale.h>


namespace had {
    Interface::Interface() {
        setlocale(LC_ALL, "");
        initscr();
        start_color();
        noecho();
        // halfdelay(1);
        timeout(0);
        cbreak();
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
