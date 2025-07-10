#include "spectre.h"

#include "Fourier_transform.h"
#include "had/had_interface.h"
#include "had/had_types.h"

#include <algorithm>
#include <string>
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
    if (input_data.empty()) {
        return had::Res::success;
    }
    FourierTransformer{}.transform(input_data, output_data);
    // output_data.clear();
    // for (const auto& it : input_data) {
    //     output_data.push_back(it);
    // }

    sp_data.clear();
    float max = std::abs(*std::max_element(
        output_data.cbegin(),
        output_data.cend(),
        [](const auto& a, const auto& b) {
            return std::abs(a) < std::abs(b);
        }
    ));
    for (int q = 0; q < std::min<had::Dem>(w, output_data.size()); ++q) {
        sp_data.push_back(
            static_cast<int>(
                std::abs(output_data[output_data.size() / w * q])
                    / max * h * had::SPECTRE_SYMBOLS_COUNT / 2
            )
        );
    }

    return had::Res::success;
}

had::Res Spectre::draw() {
    get_data(input_data);
    prepare_data(drawer.get_width(), drawer.get_height());
    drawer.cls();
    for (int q = 0; q < sp_data.size(); ++q) {
        int w = sp_data[q] / had::SPECTRE_SYMBOLS_COUNT;
        int reminder = sp_data[q] % had::SPECTRE_SYMBOLS_COUNT;
        drawer.draw_sp_symbol(
            q, drawer.get_height() - 1 - w,
            had::get_spectre_symbol_by_number(reminder)
        );
        --w;
        for (; 0 <= w; --w) {
            drawer.draw_sp_symbol(
                q, drawer.get_height() - 1 - w,
                had::SpSymbol::spectre_8_symbol
            );
        }
    }

    return had::Res::success;
}

had::Res Spectre::resize() {

    drawer.cls();

    return had::Res::success;
}
