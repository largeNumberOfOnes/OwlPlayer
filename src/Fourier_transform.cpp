#include "Fourier_transform.h"

#include <cmath>
#include <complex>
#include <cstddef>



void FourierTransformer::transform(
    const std::vector<std::complex<float>>& input,
    std::vector<std::complex<float>>& output,
    std::size_t output_size
) {
    using namespace std::complex_literals;
    using namespace std::numbers;

    output.clear();
    int N = input.size();
    for (int n = 0; n < output_size; ++n) {
        std::complex<float> sum = 0;
        for (int k = 0; k < N; ++k) {   
                sum += input[k] * std::exp(-1if *
                                    std::complex<float>(2*pi * n * k / N));
        }
        output.push_back(sum);
    }
}

#include <bit>

static std::size_t reverse_bits(size_t x, size_t n) {
    std::size_t result = 0;
    std::size_t log2n = std::bit_width(n) - 1;
    
    for (size_t i = 0; i < log2n; ++i) {
        result = (result << 1) | (x & 1);
        x >>= 1;
    }
    
    return result;
}

void FourierTransformer::transform_fast(
    const std::vector<std::complex<float>>& input,
    std::vector<std::complex<float>>& output,
    std::size_t output_size
) {
    using namespace std::numbers;
    using complex_t = std::complex<float>;

    size_t n = 1 << static_cast<int>(std::floor(std::log2(input.size())));
    if (output_size > n) {
        return;
    }
    output = input;

    // Bit-reversal permutation
    size_t log2n = std::bit_width(n) - 1;
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t j = reverse_bits(i, n);
        if (i < j) {
            std::swap(output[i], output[j]);
        }
    }

    // Итеративное вычисление FFT
    for (size_t len = 2; len <= n; len <<= 1) {
        double angle = -2 * pi / len;
        complex_t wlen(std::cos(angle), std::sin(angle));
        
        for (std::size_t i = 0; i < n; i += len) {
            complex_t w(1);
            
            for (size_t j = 0; j < len / 2; ++j) {
                complex_t u = output[i + j];
                complex_t v = w * output[i + j + len / 2];
                
                output[i + j] = u + v;
                output[i + j + len / 2] = u - v;
                
                w *= wlen;
            }
        }
    }   
}
