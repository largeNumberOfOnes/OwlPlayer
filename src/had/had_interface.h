


#pragma once

#include "had_keys.h"
#include "had_logger.h"
#include "had_types.h"
#include <curses.h>


namespace had {
    class Interface;
    class Color {
        int color;
        friend Interface;
    };

    class Interface {
        const Logger& log;

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
            Res change_color(Color& col, bool cb, int r, int g, int b);
            Res set_color(const Color& col);
            Res get_defult_color(Color& col);
            Res get_color_comp(
                const Color& col,
                short& tr, short& tg, short& tb,
                short& br, short& bg, short& bb
            );
            
            KeySequence catch_key_seq();
    };

    enum class SpSymbol {
        list_symbol,
        list_end_symbol,
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

            Res set_color(Color& color);
            Res draw_symbol(Dem x, Dem y, char ch);
            Res draw_wide_symbol(Dem x, Dem y, wchar_t ch);
            Res draw_sp_symbol(Dem x, Dem y, SpSymbol ch);
            Res draw_text(Dem x, Dem y, std::string str);
            Res draw_slider(Dem x, Dem y, Dem len, Dem val);
            Res cls();

            Res set_color(Color col);

            Dem get_width();
            Dem get_heigth();
            void set_width(Dem w);
            void set_heigth(Dem h);

    };
}