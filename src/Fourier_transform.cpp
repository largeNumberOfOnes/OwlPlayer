#include "Fourier_transform.h"

#include <cmath>
#include <complex>



void FourierTransformer::transform(
    std::vector<std::complex<float>>& input,
    std::vector<std::complex<float>>& output
) {
    using namespace std::complex_literals;
    using namespace std::numbers;

    output.clear();
    int N = input.size();
    for (int n = 0; n < N; ++n) {
        std::complex<float> sum = 0;
        for (int k = 0; k < N; ++k) {   
                sum += input[k] * std::exp(-1if *
                                    std::complex<float>(2*pi * n * k / N));
        }
        output.push_back(sum);
    }
}
