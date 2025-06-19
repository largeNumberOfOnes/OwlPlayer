#include "spectre.h"



Spectre::Spectre(had::Drawer& drawer,
                get_data_ptr get_data,
                const had::Logger& log)
    : drawer(drawer)
    , get_data(get_data)
    , log(log)
{
    // pass
}


had::Res Spectre::prepare_data() {
    return had::Res::success;
}

had::Res Spectre::draw() {
    // std::vector<int>& data = get_data();
    // std::vector<float> data = {3,2,1,4,4,6,3,2,4,6,7,3,5};

    // had::Dem h = drawer.get_height();
    // for (int q = 0; q < )



    return had::Res::success;

}

had::Res Spectre::resize() {

    return had::Res::success;
}
