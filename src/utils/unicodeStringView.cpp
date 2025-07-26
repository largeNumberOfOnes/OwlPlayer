#include "unicodeStringView.h"



namespace utils {
    UnicodeStringView::UnicodeStringView()
        : ptr(nullptr)
        , byte_len(0)
        , char_len(0)
    {}

    std::size_t UnicodeStringView::calc_byte_len(const char* ptr) {
        std::size_t byte_len = 0;
        while (ptr[byte_len] != '\0') {
            ++byte_len;
        }
        return byte_len;
    }

    std::size_t UnicodeStringView::calc_char_len(const char* ptr,
                                                    std::size_t byte_len) {
        std::size_t char_len = 0;
        for (std::size_t q = 0; q < byte_len; ++char_len) {
            char32_t c = 0;
            char* c_arr = reinterpret_cast<char*>(&c);
            for (int w = 0; w < 4; ++w) {
                c_arr[w] = (q + w < byte_len) ? ptr[q + w] : 0;
            }
            q += symbol_len(c);

        }
        return char_len;
    }

    UnicodeStringView::UnicodeStringView(const char* ptr)
        : ptr(ptr)
        , byte_len(calc_byte_len(ptr))
        , char_len(calc_char_len(ptr, byte_len))
    {}

    UnicodeStringView::UnicodeStringView(std::string_view str)
        : UnicodeStringView(str.data())
    {}

    UnicodeStringView::UnicodeStringView(const UnicodeStringView&) =
                                                                default;
    UnicodeStringView& UnicodeStringView::operator=
                                    (const UnicodeStringView&) = default;
    UnicodeStringView::UnicodeStringView(UnicodeStringView&&) = default;
    UnicodeStringView& UnicodeStringView::operator=(UnicodeStringView&&) =
                                                                default;

    UnicodeStringView::~UnicodeStringView() = default;

    std::size_t UnicodeStringView::symbol_len(char32_t c) {
        return ((c & 0x80) == 0x00) ? 1 // 1 byte - symbol
             : ((c & 0xE0) == 0xC0) ? 2 // 2 byte - symbol
             : ((c & 0xF0) == 0xE0) ? 3 // 3 byte - symbol
             : ((c & 0xF8) == 0xF0) ? 4 // 4 byte - symbol
             : 1;                       // incorrect
    }

    void UnicodeStringView::paste_symbol(char* ptr, char32_t c) {
        char* c_arr = reinterpret_cast<char*>(&c);
        for (int w = 0; w < symbol_len(c); ++w) {
            ptr[w] = c_arr[w];
        }
    }

    std::size_t UnicodeStringView::get_substr_byte_len(std::size_t
                                                substr_char_len) const {
        std::size_t sbl = 0;
        for (std::size_t q = 0; q < substr_char_len; ++q) {
            char32_t c = 0;
            char* c_arr = reinterpret_cast<char*>(&c);
            for (int w = 0; w < 4; ++w) {
                c_arr[w] = (sbl + w < byte_len) ? ptr[sbl + w] : 0;
            }
            sbl += symbol_len(c);
        }
        return sbl;
    }

    std::size_t UnicodeStringView::get_byte_len() const {
        return byte_len;
    }
    
    std::size_t UnicodeStringView::get_char_len() const {
        return char_len;
    }

    void UnicodeStringView::set_new_ptr(const char* ptr) {
        this->ptr = ptr;
        byte_len = calc_byte_len(ptr);
        char_len = calc_char_len(ptr, byte_len);
    }
}
