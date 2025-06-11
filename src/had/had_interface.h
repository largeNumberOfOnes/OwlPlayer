


#pragma once

#include "had_logger.h"
#include "had_types.h"
#include <curses.h>


namespace had {

    class Color {

    };

    class Interface {

        public:
            Interface();
            ~Interface();

            Dem get_width();
            Dem get_height();

            Res update();
            Res cls();
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
            Res draw_text(Dem x, Dem y, std::string str);
            Res draw_slider(Dem x, Dem y, Dem len, Dem val);
            Res cls();

            Dem get_width();
            Dem get_heigth();
            void set_width(Dem w);
            void set_heigth(Dem h);

    };
}