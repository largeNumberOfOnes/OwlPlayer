#include "../had.h"



class Player {

    static had::dem const min_h_to_draw_topPannel = 4;
    static had::dem const vol_min_width = 12;
    static had::dem const tim_min_width = 26;
    static had::dem const vol_tim_pad = 3;
    static had::dem const min_width = vol_min_width + tim_min_width +
                                                        vol_tim_pad;

    public:
        // static had::dem const min_w = 0;
        static had::dem const min_h = 4;
        // pass

        stat draw(had::dem x, had::dem y, had::dem w) {return stat::success; }

        stat stop();
        stat play();
};
