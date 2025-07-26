#include "unicodeString.h"

#include "unicodeStringView.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <string_view>



namespace utils {

    UnicodeString::UnicodeString()
        : UnicodeStringView()
        , buf_len(0)
    {}

    UnicodeString::UnicodeString(const char* str)
        : UnicodeStringView(str)
    {
        buf_len = byte_len + 1;
        ptr = new char[buf_len];
        mem_copy(str, deconst_ptr(), buf_len);
    }

    UnicodeString::UnicodeString(std::string_view str)
        : UnicodeString(str.data())
    {}

    UnicodeString::UnicodeString(const UnicodeString& str)
        : UnicodeString(str.ptr)
    {}
    
    UnicodeString& UnicodeString::operator=(const UnicodeString& str) {
        if (str.byte_len + 1 > buf_len) {
            delete [] ptr;
            byte_len = str.byte_len;
            char_len = str.char_len;
            buf_len  = str.byte_len + 1;
            ptr = new char[buf_len];
            mem_copy(str.ptr, deconst_ptr(), buf_len);
        } else {
            byte_len = str.byte_len;
            char_len = str.char_len;
            mem_copy(str.ptr, deconst_ptr(), byte_len + 1);
        }
        return *this;
    }

    UnicodeString::UnicodeString(UnicodeString&& str)
        : UnicodeStringView(nullptr)
    {
        ptr      = str.ptr;
        byte_len = str.byte_len;
        char_len = str.char_len;
        buf_len  = str.buf_len;

        str.ptr      = nullptr;
        str.byte_len = 0;
        str.char_len = 0;
        str.buf_len  = 0;
    }

    UnicodeString& UnicodeString::operator=(UnicodeString&& str) {
        delete [] ptr;

        ptr      = str.ptr;
        byte_len = str.byte_len;
        char_len = str.char_len;
        buf_len  = str.buf_len;

        str.ptr      = nullptr;
        str.byte_len = 0;
        str.char_len = 0;
        str.buf_len  = 0;

        return *this;
    }
    
    UnicodeString::~UnicodeString() {
        delete [] ptr;
    }

    inline void UnicodeString::mem_copy(const char* src, char* dst,
                                                        std::size_t len) {
        for (std::size_t q = 0; q < len; ++q) {
            dst[q] = src[q]; // DEV [optimize?]
        }
    }

    void UnicodeString::insert_symbol_by_char_pos(std::size_t char_pos,
                                                        char32_t symbol) {
        std::size_t symbol_len = UnicodeStringView::symbol_len(symbol);
        std::size_t byte_pos = get_substr_byte_len(char_pos);
        if (symbol_len + byte_len + 1 <= buf_len) {
            mem_copy(
                ptr + symbol_len,
                deconst_ptr() + byte_pos + symbol_len,
                byte_len - byte_pos
            );
            UnicodeStringView::paste_symbol(
                                        deconst_ptr() + byte_pos, symbol);
        } else {
            buf_len *= 2;
            char* new_ptr = new char[buf_len];
            mem_copy(ptr, new_ptr, byte_pos);
            mem_copy(
                ptr + symbol_len,
                new_ptr + byte_pos + symbol_len,
                byte_len - byte_pos
            );
            UnicodeStringView::paste_symbol(new_ptr + byte_pos, symbol);
            delete [] ptr;
            ptr = new_ptr;
        }
        byte_len += symbol_len;
        char_len += 1;
    }

    std::string_view UnicodeString::to_string_view() const {
        return ptr;
    }
}
