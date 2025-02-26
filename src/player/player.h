#include "../had.h"



class Player {

    static had::dem const vol_min_width = 12;
    static had::dem const vol_max_width = 20;
    static had::dem const tim_min_width = 26;
    static had::dem const tim_half_width = 40;
    static had::dem const vol_tim_pad = 3;
    static had::dem const min_width = vol_min_width + tim_min_width +
                                                            vol_tim_pad;
    static had::dem const min_height = 5;

    had::volume vol;

    public:
        // static had::dem const min_w = 0;
        static had::dem const min_h = 4;

        res draw(had::dem x, had::dem y, had::dem w);

        res stop();
        res play();
};
