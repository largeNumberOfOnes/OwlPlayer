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
    int output_count = w;
    FourierTransformer{}.transform_fast(input_data[0], output_data, output_count);
    log.log_info(std::to_string(output_data.size()));

    sp_data.clear();
    int count = std::min<had::Dem>(w, output_data.size());
    for (int q = 0; q < count; ++q) {
        sp_data.push_back(
            static_cast<int>(
                std::abs(output_data[output_data.size() / w * q])
            )
        );
    }
    float max = *std::max_element(sp_data.cbegin(), sp_data.cend());
    for (auto& it : sp_data) {
        it *= 1 / max * h * had::SPECTRE_SYMBOLS_COUNT * 3 / 4;
    }

    return had::Res::success;
}

had::Res Spectre::draw() {
    get_data(input_data);
    prepare_data(drawer.get_width(), drawer.get_height());
    drawer.cls();
    int rrr = 3;
    int ttt = 1;
    for (int q = 0; q < sp_data.size(); ++q) {
        int ind = q - q % rrr;
        if (q - ind < ttt) {
            continue;
        }
        int w = sp_data[ind] / had::SPECTRE_SYMBOLS_COUNT;
        int reminder = sp_data[ind] % had::SPECTRE_SYMBOLS_COUNT;
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
    return had::Res::success;
}
