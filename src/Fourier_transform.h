#include <complex>
#include <vector>
#include <cmath>



class FourierTransformer {
    public:
        void transform(
            std::vector<std::complex<float>>& input,
            std::vector<std::complex<float>>& output
        );
};
