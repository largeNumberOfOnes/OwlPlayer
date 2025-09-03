/**
 * pass
 */

#pragma once

#include "had_types.h"

#include <complex>
#include <vector>



namespace had {
    class AudioFile; // DEV

    class DataFrame {
        std::size_t id;
        using DataType = std::vector<
            std::vector<std::complex<float>>
        >;
        DataType data;
        int rate = 0;
        SampleDem start = 0;

        DataFrame(std::size_t id);
        friend AudioFile;
        
        public:
            DataFrame(const DataFrame&) = delete;
            DataFrame& operator=(const DataFrame&) = delete;
            DataFrame(DataFrame&&);
            DataFrame& operator=(DataFrame&&);
            
            Res push(DataType&& data, int rate, SampleDem start);
            int get_channels_count();
    };

}
