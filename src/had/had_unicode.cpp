#include "had_unicode.h"



namespace had {
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
            int c = 0;
            char* c_arr = reinterpret_cast<char*>(&c);
            for (int w = 0; w < 4; ++w) {
                c_arr[w] = (q + w < byte_len) ? ptr[q + w] : 0;
            }
            q += ((c & 0x80) == 0x00) ? 1 // 1 byte - symbol
               : ((c & 0xE0) == 0xC0) ? 2 // 2 byte - symbol
               : ((c & 0xF0) == 0xE0) ? 3 // 3 byte - symbol
               : ((c & 0xF8) == 0xF0) ? 4 // 4 byte - symbol
               : 1;                       // incorrect

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

    std::size_t UnicodeStringView::get_substr_byte_len(std::size_t
                                                substr_char_len) const {
        std::size_t sbl = 0;
        for (std::size_t q = 0; q < substr_char_len; ++q) {
            int c = 0;
            char* c_arr = reinterpret_cast<char*>(&c);
            for (int w = 0; w < 4; ++w) {
                c_arr[w] = (sbl + w < byte_len) ? ptr[sbl + w] : 0;
            }
            sbl += ((c & 0x80) == 0x00) ? 1 // 1 byte - symbol
                 : ((c & 0xE0) == 0xC0) ? 2 // 2 byte - symbol
                 : ((c & 0xF0) == 0xE0) ? 3 // 3 byte - symbol
                 : ((c & 0xF8) == 0xF0) ? 4 // 4 byte - symbol
                 : 1;                       // incorrect

        }
        return sbl;
    }

    std::size_t UnicodeStringView::get_byte_len() const {
        return byte_len;
    }
    
    std::size_t UnicodeStringView::get_char_len() const {
        return char_len;
    }
}
