#include "../had/had.h"



class Player {

    had::volume vol;

    public:
        bool not_enougth_space(had::dem w, had::dem h);
        had::dem get_height();

        res draw(had::dem x, had::dem y, had::dem w);

        res stop();
        res play();
};
