


#pragma once

#include "had_keys.h"
#include "had_logger.h"
#include "had_types.h"
#include <curses.h>
#include <string_view>


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
            Interface(const Interface&) = delete;
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
            Res create_background_color(
                Color& col,
                int br, int bg, int bb
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
        scrol_up_symbol,
        scrol_middle_symbol,
        scrol_bottom_symbol,
        back_arrow_symbol,
        
        spectre_0_symbol,
        spectre_1_symbol,
        spectre_2_symbol,
        spectre_3_symbol,
        spectre_4_symbol,
        spectre_5_symbol,
        spectre_6_symbol,
        spectre_7_symbol,
        spectre_8_symbol,
    };
    inline SpSymbol get_spectre_symbol_by_number(int n) {
        switch (n) {
            case 0:  return SpSymbol::spectre_0_symbol;
            case 1:  return SpSymbol::spectre_1_symbol;
            case 2:  return SpSymbol::spectre_2_symbol;
            case 3:  return SpSymbol::spectre_3_symbol;
            case 4:  return SpSymbol::spectre_4_symbol;
            case 5:  return SpSymbol::spectre_5_symbol;
            case 6:  return SpSymbol::spectre_6_symbol;
            case 7:  return SpSymbol::spectre_7_symbol;
            default: return SpSymbol::spectre_8_symbol;
        }
    }
    constexpr int SPECTRE_SYMBOLS_COUNT = 8;

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
            Drawer(
                Interface& interface,
                const had::Logger&
            );
            Drawer(const Drawer&) = default;
            Drawer(Drawer&&)      = default;
            ~Drawer()             = default;

            void set(Dem x, Dem y, Dem w, Dem h);

            Res draw_symbol(Dem x, Dem y, char ch);
            Res draw_symbol_char32(Dem x, Dem y, char32_t ch);
            Res draw_wide_symbol(Dem x, Dem y, wchar_t ch);
            Res draw_sp_symbol(Dem x, Dem y, SpSymbol ch);
            Res draw_text(Dem x, Dem y, std::string_view str);
            Res draw_text_n(Dem x, Dem y, std::string_view str, int n);
            Res draw_slider(Dem x, Dem y, Dem len, Dem val);
            Res cls();

            Res set_color(const Color& col);

            Dem get_width();
            Dem get_height();
            void set_width(Dem w);
            void set_heigth(Dem h);

    };
}