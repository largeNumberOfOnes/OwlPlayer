



#include "had/had.h"

#include <complex>
#include <vector>


class Spectre {
    had::Drawer& drawer;
    const had::Logger& log;
    using get_data_ptr = std::function<std::vector<int>&(void)>;
    get_data_ptr get_data;

    std::vector<std::complex<double>> data;

    had::Res prepare_data();

    public:
        Spectre(had::Drawer& drawer,
                get_data_ptr get_data,
                const had::Logger& log);

        had::Res draw();
        had::Res resize();
};
