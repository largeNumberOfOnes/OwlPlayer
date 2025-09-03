#include "had_dataFrame.h"


namespace had {
    
    DataFrame::DataFrame(std::size_t id) : id(id) {}
    DataFrame::DataFrame(DataFrame&&) = default;
    DataFrame& DataFrame::operator=(DataFrame&&) = default;

    Res DataFrame::push(
        std::vector<
            std::vector<std::complex<float>>
        >&& data,
        int rate,
        SampleDem start
    ) {
        if (data.size() == 0) {
            return Res::error;
        }
        for (const auto& it : data) {
            if (data[0].size() != it.size()) {
                return Res::error;
            }
        }
        this->data  = std::move(data);
        this->rate  = rate;
        this->start = start;
        return Res::success;
    }

    int DataFrame::get_channels_count() {
        return data.size();
    }

}
