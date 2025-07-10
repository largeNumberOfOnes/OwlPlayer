#pragma once

#include "had/had.h"

#include <complex>
#include <vector>



class Spectre {
    public:
        using DataArray = std::vector<std::complex<float>>;
        using GetDataPtr =
                std::function<void(DataArray&)>;
    private:
        had::Drawer& drawer;
        const had::Logger& log;
        using SpData = std::vector<int>;
        GetDataPtr get_data;

        DataArray input_data;
        DataArray output_data;
        SpData sp_data;

        had::Res prepare_data(had::Dem w, had::Dem h);

    public:
        Spectre(had::Drawer& drawer,
                GetDataPtr get_data,
                const had::Logger& log);

        had::Res draw();
        had::Res resize();
};
