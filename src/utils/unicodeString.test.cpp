#include "unicodeString.h"

#include <iostream>
#include <istream>
#include <optional>
#include <cassert>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::optional<T>& opt) {
    if (opt.has_value()) {
        out << opt.value();
    } else {
        out << "std::nullopt";
    }
    return out;
}

void test_unicode_support() {
    using namespace utils::unicode_support;

    assert(calc_byte_len(nullptr) == 0);
    assert(calc_byte_len("") == 0);
    assert(calc_byte_len("hello") == 5);
    assert(calc_byte_len("h\u00a9 \u0156 7 \u03567") == 12);
    assert(calc_byte_len("h\U0001F600") == 5);

    assert(calc_char_len(nullptr) == 0);
    assert(calc_char_len("") == 0);
    assert(calc_char_len("hello") == 5);
    assert(calc_char_len("h\u00a9 \u0156 7 \u03567") == 9);
    assert(calc_char_len("h\U0001F600") == 2);

    assert(calc_substr_byte_len(nullptr, 7) == 0);
    assert(calc_substr_byte_len("", 7) == 0);
    assert(calc_substr_byte_len("qwertyuiop", 5) == 5);
    assert(calc_substr_byte_len("qwertyuiop", 77) == 10);
    assert(calc_substr_byte_len("h\u00a9 \u0156 7 \u03567", 4) == 6);
    assert(calc_substr_byte_len("h\u00a9 \u0156 7 \u03567", 10) == 12);

    const char* str1 = nullptr;
    const char* str2 = "";
    const char* str3 = "ajdoe_nfeodkfhd_hkenf_ldx"; // 5 15 21
    const char* str4 = "ajd\xc2\xde_nfeofhd_h\xc5\x93kenf_ldx"; // 4 12 19

    assert(rfind_till(str1, '_',   77) == std::nullopt);
    assert(rfind_till(str2, '_',   77) == std::nullopt);
    assert(rfind_till(str3, '_',   77) == 5);
    assert(rfind_till(str3, '_',    3) == std::nullopt);
    assert(rfind_till(str3, '_',    5) == std::nullopt);
    assert(rfind_till(str4, '_',   77) == 4);
    assert(rfind_till(str4, '_',    2) == std::nullopt);
    assert(rfind_till(str4, 0xdec2, 7) == 3);

    assert(lfind_from(str1, '_',     0) == std::nullopt);
    assert(lfind_from(str2, '_',     0) == std::nullopt);
    assert(lfind_from(str3, '_',    10) == 5);
    assert(lfind_from(str3, '_',    20) == 15);
    assert(lfind_from(str3, '_',    77) == 21);
    assert(lfind_from(str3, '_',     4) == std::nullopt);
    assert(lfind_from(str3, '_',     5) == std::nullopt);
    assert(lfind_from(str4, '_',    77) == 19);
    assert(lfind_from(str4, '_',     2) == std::nullopt);
    assert(lfind_from(str4, 0xdec2, 77) == 3);
    assert(lfind_from(str4, 0xdec2,  3) == std::nullopt);
    assert(lfind_from(str4, 0xdec2,  2) == std::nullopt);

    // assert(rfind_till(nullptr, '7', 5) == std::nullopt);
    // assert(rfind_till("", '7', 4) == std::nullopt);
    // assert(rfind_till("", '7', 0) == std::nullopt);
    // assert(rfind_till("hello", '3', 0) == std::nullopt);
    // assert(rfind_till("hellol", 'l', 0) == std::nullopt);
    // assert(rfind_till("hellol", 'l', 3) == 2);
    // assert(rfind_till("hello", 'l', 77) == 3);
    // assert(rfind_till("h\u00a9 \u0156 7 \u03567", '7', 7) == 5);
    // assert(rfind_till("h\u00a9 \u0156 7 \u03567", ch2, 77)
    //                                                     == std::nullopt);
    // assert(rfind_till("h\u00a9 \u0156 7 \u03567", ch1, 5) == 3);
    // assert(rfind_till("h\U0001F600", ch2, 77) == 1);

    // assert(lfind_from(nullptr, '7', 5) == std::nullopt);
    // assert(lfind_from("", '7', 4) == std::nullopt);
    // assert(lfind_from("", '7', 0) == std::nullopt);
    // assert(lfind_from("hello", '3', 3) == std::nullopt);
    // assert(lfind_from("hellol", 'l', 0) == std::nullopt);
    // assert(lfind_from("hellol", 'l', 5) == 3);
    // assert(lfind_from("h\u00a9 \u0156 7 \u03567", '7', 7) == 5);
    // assert(lfind_from("h\u00a9 \u0156 7 \u03567", ch2, 77)
    //                                                     == std::nullopt);
    // assert(lfind_from("h\u00a9 \u0156 7 \u03567", ch1, 5) == std::nullopt);
    // assert(lfind_from("h\U0001F600", ch2, 77) == 1);
}

int test_unicodeStringView() {
    std::string str("9↪эти");
    std::size_t exp_byte_len = 10;
    std::size_t exp_char_len = 5;

    utils::UnicodeStringView uni_str{str};

    std::cout << "Start test" << std::endl;
    assert(uni_str.get_byte_len() == exp_byte_len);
    assert(uni_str.get_char_len() == exp_char_len);
    // assert(uni_str.get_substr_byte_len(1) == 1);
    // assert(uni_str.get_substr_byte_len(2) == 4);
    // assert(uni_str.get_substr_byte_len(3) == 6);
    // assert(uni_str.get_substr_byte_len(4) == 8);
    // assert(uni_str.get_substr_byte_len(5) == 10);
    std::cout << "Correct" << std::endl;

    return 0;
}

int main() {
    test_unicode_support();

    return 0;
}
