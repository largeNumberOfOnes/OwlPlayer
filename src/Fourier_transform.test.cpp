#include "Fourier_transform.h"

#include <iostream>
#include <vector>



template <typename T>
requires requires(T t) {
    std::cout << t;
}
void print_vec(
    std::vector<T>& vec,
    std::string sep = " ",
    std::string end = "\n"
) {
    for (const auto& it : vec) {
        std::cout << it << sep;
    }
    std::cout << end;
}

void test1() {
    FourierTransformer transformer;

    std::vector<std::complex<float>> vec{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<std::complex<float>> output1;
    std::vector<std::complex<float>> output2;
    std::vector<std::complex<float>> output3;
    std::size_t count = 4;

    transformer.transform(vec, output1, count);
    transformer.transform_fast(vec, output3, count);

    print_vec(vec);
    print_vec(output1);
    print_vec(output2);
    print_vec(output3);
}

int main() {
    test1();

    return 0;
}