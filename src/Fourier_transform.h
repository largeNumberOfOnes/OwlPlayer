#include <complex>
#include <vector>
#include <cmath>



class FourierTransformer {
    public:
        FourierTransformer();
        ~FourierTransformer();

        void transform(
            std::vector<std::complex<float>>& input,
            std::vector<std::complex<float>>& output
        );

};
