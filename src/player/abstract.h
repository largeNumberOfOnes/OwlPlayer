#pragma once

#include <string>
#include <iostream>



// enum Key {

// };

using dem = unsigned int;
using seconds = unsigned int;
using volume = unsigned int;
enum stat {
    success = 0,
    error   = 1,
};

enum class key {
    arrow_rigth,
    arrow_left,
    arrow_up,
    arrow_down,
    nothing,
};

class Abstract {
    private:
        Abstract(Abstract const&) = delete;
        void operator=(Abstract const&) = delete;

    public:
        Abstract();
        ~Abstract();
        // static Abstract& get_instance();

        // Cathing symbols
        key get_key();

        // Interface
        void set_color();
        void move();
        stat draw_slider(dem x, dem y, dem len, dem val);
        stat draw_text(dem x, dem y, std::string const& str);
        stat update();
        stat cls();
        dem get_width();
        dem get_height();

        // AudioPlayer
        stat load(std::string const& path);
        stat remove();
        seconds get_duration();
        seconds get_cur_time();
        void jump(int pos); // pos in seconds

        std::string get_config_path();

};
