#include <complex>
#include <vector>
#include <cmath>



class FourierTransformer {
    public:
        void transform(
            const std::vector<std::complex<float>>& input,
            std::vector<std::complex<float>>& output,
            std::size_t output_size
        );

        void transform_fast(
            const std::vector<std::complex<float>>& input,
            std::vector<std::complex<float>>& output,
            std::size_t output_size
        );

        void transform_fast_(
            const std::vector<std::complex<float>>& input,
            std::vector<std::complex<float>>& output,
            std::size_t output_size
        );
};
