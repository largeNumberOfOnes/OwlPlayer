#include "unicodeStringView.h"

#include <iostream>
#include <cassert>



int main() {
    std::string str("9↪эти");
    std::size_t exp_byte_len = 10;
    std::size_t exp_char_len = 5;

    utils::UnicodeStringView uni_str{str};

    std::cout << "Start test" << std::endl;
    assert(uni_str.get_byte_len() == exp_byte_len);
    assert(uni_str.get_char_len() == exp_char_len);
    assert(uni_str.get_substr_byte_len(1) == 1);
    assert(uni_str.get_substr_byte_len(2) == 4);
    assert(uni_str.get_substr_byte_len(3) == 6);
    assert(uni_str.get_substr_byte_len(4) == 8);
    assert(uni_str.get_substr_byte_len(5) == 10);
    std::cout << "Correct" << std::endl;

    return 0;
}
