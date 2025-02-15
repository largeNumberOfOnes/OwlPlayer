#include <chrono>
#include <curses.h>
#include <ncurses.h>
#include <iostream>
#include <thread>
// #include <locale.h>
// #include <wchar.h>

int main() {
    // setlocale(LC_ALL, "C-UTF-8");
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    
    // auto a = COLOR_PAIRS;

    // std::cout << "COLOR_PAIRS = " << COLOR_PAIRS << std::endl;
    // std::cout << "COLORS = " << COLORS << std::endl;
    // getch();

    // wchar_t ch = '■';
    // int w, h;
    // getmaxyx(stdscr, h, w);
    // int count = w;
    // int color_start = 20;
    // int pair_start = 20;
    // for (int q = 0; q < count; ++q) {
    //     double col = (double)q/count * 1000;
    //     // std::cout << col << std::endl;
    //     init_color(color_start + q, col, col, col);
    //     init_pair(pair_start + q, color_start + q, COLOR_BLACK);
    //     // init_pair(pair_start + q, COLOR_BLACK, color_start + q);
    // }

    // for (int q = 0; q < count; ++q) {
    //     for (int e = 0; e < w; ++e) {
    //         move(e, q);
    //         // attron(COLOR_PAIR(pair_start + q));

    //         // cchar_t ch = {COLOR_PAIR(pair_start + q), L'\u25A0'};
    //         wchar_t ch = L'\u25A0';
    //         // cchar_t ch = {COLOR_PAIR(pair_start + q), L'☺'};
    //         // add_wch(&ch);
    //         // attr
    //         // attroff(COLOR_PAIR(pair_start + q));
    //     }
    // }

    int w = 0, h = 0;
    int point_count = 1000;
    int colors_count = 0;
    getmaxyx(stdscr, h, w);
    while (true) {
        // clear();
        for (int q = 0; q < point_count; ++q) {
            int x = rand() % w, y = rand() % h;
            int r = rand() % 1000, g = rand() % 1000, b = rand() % 1000;
            move(y, x);
            init_color(20 + colors_count, r, g, b);
            init_pair(20 + colors_count, COLOR_BLACK, 20 + colors_count);
            addch(' ' | COLOR_PAIR(20 + colors_count));
            ++colors_count;
            refresh();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    // std::cout << "shf" << std::endl;
    }
    getch();
    
    endwin();

    return 0;
}

