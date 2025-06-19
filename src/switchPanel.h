/**
 * This class allows using 
 */

#pragma once

#include "had/had.h"



class SwitchPanel {

    int count = 0;
    int cur = 0;

    public:
        SwitchPanel();
        ~SwitchPanel();

        void inc();
        void dec();
        void set(int ch);

        had::Res draw();
        had::Res resize();
};
