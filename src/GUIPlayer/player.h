#include "../had/had.h"



class Player {

    had::volume vol;

    public:
        had::dem get_height();

        bool is_enougth_space(had::dem w, had::dem h);
        res draw(had::dem x, had::dem y, had::dem w);

        res stop();
        res play();
};
