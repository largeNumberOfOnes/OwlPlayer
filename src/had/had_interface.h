


#pragma once

#include "had_keys.h"
#include "had_logger.h"
#include "had_types.h"
#include <curses.h>


namespace had {
    class Interface;
    class Drawer;

    class Color {
        int color;
        friend Interface;
    };

    class Interface {
        Dem old_w = 0;
        Dem old_h = 0;
        const Logger& log;

        Res set_default_color();
        public:
            Interface(const Logger& log);
            ~Interface();


            Dem get_width();
            Dem get_height();

            Res update();
            Res cls();

            Res create_color(
                Color& col,
                int tr, int tg, int tb,
                int br, int bg, int bb
            );
            Res create_text_color(
                Color& col,
                int tr, int tg, int tb
            );
            Res change_color(Color& col, bool cb, int r, int g, int b);
            Res set_color(const Color& col);
            Res get_default_color(Color& col);
            
            KeySequence catch_key_seq();
            Drawer produce_drawer();
            bool is_resized();
    };

    enum class SpSymbol {
        list_symbol,
        list_end_symbol,
        list_line_symbol,
    };

    class Drawer {
        Interface& interface;
        const had::Logger& log;

        had::Dem x;
        had::Dem y;
        had::Dem w;
        had::Dem h;

        public:
            Drawer(
                Interface& interface,
                Dem x, Dem y, Dem w, Dem h,
                const had::Logger&
            );

            void set(Dem x, Dem y, Dem w, Dem h);

            Res draw_symbol(Dem x, Dem y, char ch);
            Res draw_wide_symbol(Dem x, Dem y, wchar_t ch);
            Res draw_sp_symbol(Dem x, Dem y, SpSymbol ch);
            Res draw_text(Dem x, Dem y, std::string str);
            Res draw_slider(Dem x, Dem y, Dem len, Dem val);
            Res cls();

            Res set_color(const Color& col);

            Dem get_width();
            Dem get_height();
            void set_width(Dem w);
            void set_heigth(Dem h);

    };
}