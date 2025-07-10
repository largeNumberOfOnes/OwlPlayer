#include "spectre.h"

#include "Fourier_transform.h"
#include "had/had_interface.h"
#include "had/had_types.h"

#include <algorithm>
#include <vector>



Spectre::Spectre(had::Drawer& drawer,
                GetDataPtr get_data,
                const had::Logger& log)
    : drawer(drawer)
    , get_data(get_data)
    , log(log)
{
    // pass
}

had::Res Spectre::prepare_data(had::Dem w, had::Dem h) {

    DataArray output_data;
    FourierTransformer{}.transform(input_data, output_data);

    sp_data.clear();
    for (int q = 0; q < std::min<had::Dem>(w, output_data.size()); ++q) {
        sp_data.push_back(
            static_cast<int>(std::abs(output_data[q]))
        );
    }

    return had::Res::success;
}

had::Res Spectre::draw() {
    get_data(input_data);
    prepare_data(drawer.get_width(), drawer.get_height());

    for (int q = 0; q < sp_data.size(); ++q) {
        drawer.draw_sp_symbol(q, 0, had::SpSymbol::spectre_4_symbol);
    }

    return had::Res::success;
}

had::Res Spectre::resize() {

    drawer.cls();

    return had::Res::success;
}
