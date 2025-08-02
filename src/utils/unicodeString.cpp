#include "unicodeString.h"

#include <cstddef>
#include <string_view>
#include <optional>
#include <utility>



namespace utils::unicode_support {
    inline static bool is_1b_uni_seq(int ch)
        { return ((ch & 0x80) == 0x00); };
    inline static bool is_2b_uni_seq(int ch)
        { return ((ch & 0xE0) == 0xC0); };
    inline static bool is_3b_uni_seq(int ch)
        { return ((ch & 0xF0) == 0xE0); };
    inline static bool is_4b_uni_seq(int ch)
        { return ((ch & 0xF8) == 0xF0); };

    static constexpr char32_t get_symbol(const char* ptr) {
        if (!ptr) {
            return 0;
        }
        char32_t ret = 0;
        char* arr = reinterpret_cast<char*>(&ret);
        char test = ptr[0];
        arr[0] = test;
        if (is_1b_uni_seq(test)) {
            // Nothing needs to be done
        } else if (is_2b_uni_seq(test)) {
            arr[1] = ptr[1];
        } else if (is_3b_uni_seq(test)) {
            arr[1] = ptr[1];
            arr[2] = ptr[2];
        } else if (is_4b_uni_seq(test)) {
            arr[1] = ptr[1];
            arr[2] = ptr[2];
            arr[3] = ptr[3];
        }
        return ret;
    }

    std::size_t calc_byte_len(const char* ptr) {
        if (!ptr) {
            return 0;
        }
        std::size_t byte_len = 0;
        while (ptr[byte_len] != '\0') {
            ++byte_len;
        }
        return byte_len;
    }

    std::size_t calc_char_len(const char* ptr) {
        if (!ptr) {
            return 0;
        }
        std::size_t char_len = 0;
        std::size_t byte_len = calc_byte_len(ptr);
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

    std::size_t calc_char_len_by_byte_len(const char* ptr,
                                                    std::size_t byte_len) {
        if (!ptr) {
            return 0;
        }
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

    std::size_t symbol_len(char32_t c) {
        // return ((c & 0x80) == 0x00) ? 1 // 1 byte - symbol
        //      : ((c & 0xE0) == 0xC0) ? 2 // 2 byte - symbol
        //      : ((c & 0xF0) == 0xE0) ? 3 // 3 byte - symbol
        //      : ((c & 0xF8) == 0xF0) ? 4 // 4 byte - symbol
        //      : 1;                       // incorrect
        if      (is_1b_uni_seq(c)) { return 1; }
        else if (is_2b_uni_seq(c)) { return 2; }
        else if (is_3b_uni_seq(c)) { return 3; }
        else if (is_4b_uni_seq(c)) { return 4; }
        return 1;
    }

    void paste_symbol(char* ptr, char32_t c) {
        char* c_arr = reinterpret_cast<char*>(&c);
        for (int w = 0; w < symbol_len(c); ++w) {
            ptr[w] = c_arr[w];
        }
    }

    std::size_t calc_substr_byte_len(const char *ptr,
                                            std::size_t substr_char_len) {
        std::size_t sbl = 0;
        std::size_t byte_len = calc_byte_len(ptr);
        for (std::size_t q = 0; q < substr_char_len; ++q) {
            if (byte_len <= sbl) {
                return byte_len;
            }
            char32_t c = 0;
            char* c_arr = reinterpret_cast<char*>(&c);
            for (int w = 0; w < 4; ++w) {
                c_arr[w] = (sbl + w < byte_len) ? ptr[sbl + w] : 0;
            }
            sbl += unicode_support::symbol_len(c);
        }
        return sbl;
    }

    std::optional<std::size_t> rfind_till(
        const char* ptr,
        char32_t ch,
        std::size_t till_char
    ) {
        if (!ptr) {
            return std::nullopt;
        }
        std::size_t pos = 0;
        for (;;) {
            char32_t c = get_symbol(ptr);
            if (c == '\0' || pos == till_char) {
                break;
            }
            if (c == ch) {
                return pos;
            }
            ++pos;
            ptr += symbol_len(c);
        }
        return std::nullopt;
    }
    
    std::optional<std::size_t> lfind_from(const char* ptr, char32_t ch,
                                                std::size_t from_char) {
        if (!ptr) {
            return std::nullopt;
        }
        std::optional<std::size_t> find = std::nullopt;
        std::size_t pos = 0;
        for (;;) {
            char32_t c = get_symbol(ptr);
            if (c == '\0' || pos == from_char) {
                break;
            }
            if (c == ch) {
                find = pos;
            }
            ++pos;
            ptr += symbol_len(c);
        }
        return find;
    }

    static char32_t cyrillic_symbol_to_lower(char32_t ch) {
        switch (ch) {
            case 0xb0d0: return 0x90d0; // а
            case 0xb1d0: return 0x91d0; // б
            case 0xb2d0: return 0x92d0; // в
            case 0xb3d0: return 0x93d0; // г
            case 0xb4d0: return 0x94d0; // д
            case 0xb5d0: return 0x95d0; // е
            case 0x91d1: return 0x81d0; // ё
            case 0xb6d0: return 0x96d0; // ж
            case 0xb7d0: return 0x97d0; // з
            case 0xb8d0: return 0x98d0; // и
            case 0xb9d0: return 0x99d0; // й
            case 0xbad0: return 0x9ad0; // к
            case 0xbbd0: return 0x9bd0; // л
            case 0xbcd0: return 0x9cd0; // м
            case 0xbdd0: return 0x9dd0; // н
            case 0xbed0: return 0x9ed0; // о
            case 0xbfd0: return 0x9fd0; // п
            case 0x80d1: return 0xa0d0; // р
            case 0x81d1: return 0xa1d0; // с
            case 0x82d1: return 0xa2d0; // т
            case 0x83d1: return 0xa3d0; // у
            case 0x84d1: return 0xa4d0; // ф
            case 0x85d1: return 0xa5d0; // х
            case 0x86d1: return 0xa6d0; // ц
            case 0x87d1: return 0xa7d0; // ч
            case 0x88d1: return 0xa8d0; // ш
            case 0x89d1: return 0xa9d0; // щ
            case 0x8ad1: return 0xaad0; // ъ
            case 0x8bd1: return 0xabd0; // ы
            case 0x8cd1: return 0xacd0; // ь
            case 0x8dd1: return 0xadd0; // э
            case 0x8ed1: return 0xaed0; // ю
            case 0x8fd1: return 0xafd0; // я
        };
        return ch;
    }

    char32_t symbol_to_lower(char32_t ch) {
        if ('A' <= ch && ch <= 'Z') {
            return ch - 'A' + 'a';
        }
        return cyrillic_symbol_to_lower(ch);
    }
}

namespace utils { // ConstUnicodeStringIter
    ConstUnicodeStringIter::ConstUnicodeStringIter(
        const char* ptr,
        std::size_t byte_len,
        std::size_t byte_pos
    )
        : ptr(ptr)
        , byte_len(byte_len)
        , byte_pos(byte_pos)
    {}

    ConstUnicodeStringIter::ConstUnicodeStringIter(
        const ConstUnicodeStringIter& other
    ) : ConstUnicodeStringIter(other.ptr, other.byte_len, other.byte_pos)
    {}

    ConstUnicodeStringIter& ConstUnicodeStringIter::operator=
                            (const ConstUnicodeStringIter& other) {
        ptr = other.ptr;
        byte_len = other.byte_len;
        byte_pos = other.byte_pos;
        return *this;
    }

    std::size_t ConstUnicodeStringIter::get_char_pos() const {
        return unicode_support::calc_char_len_by_byte_len(ptr, byte_pos);
    }

    ConstUnicodeStringIter& ConstUnicodeStringIter::operator++() {
        if (byte_pos == byte_len) {
            return *this;
        }
        char32_t ch = unicode_support::get_symbol(ptr + byte_len);
        byte_pos += unicode_support::symbol_len(ch);
        return *this;
    }

    bool ConstUnicodeStringIter::operator==(
        const ConstUnicodeStringIter& other
    ) const {
        return ptr == other.ptr
            && byte_len == other.byte_len
            && byte_pos == other.byte_pos;
    }

    char32_t ConstUnicodeStringIter::operator*() const {
        if (!ptr) {
            return 0;
        }
        return unicode_support::get_symbol(ptr + byte_pos);
    }

}

namespace utils { // UnicodeStringView
    UnicodeStringView::UnicodeStringView()
        : ptr(nullptr)
        , byte_len(0)
        , char_len(0)
    {}

    UnicodeStringView::UnicodeStringView(const char* ptr)
        : ptr(ptr)
        , byte_len(unicode_support::calc_byte_len(ptr))
        , char_len(unicode_support::calc_char_len(ptr))
    {}

    UnicodeStringView::UnicodeStringView(std::string_view str)
        : UnicodeStringView(str.data())
    {}

    UnicodeStringView::UnicodeStringView(const UnicodeStringView&)
                                                                = default;
                                                                
    UnicodeStringView& UnicodeStringView::operator=
                                    (const UnicodeStringView&) = default;

    UnicodeStringView::UnicodeStringView(UnicodeStringView&&) = default;

    UnicodeStringView& UnicodeStringView::operator=(UnicodeStringView&&)
                                                                = default;

    UnicodeStringView::~UnicodeStringView() = default;

    std::size_t UnicodeStringView::get_byte_len() const {
        return byte_len;
    }
    
    std::size_t UnicodeStringView::get_char_len() const {
        return char_len;
    }

    const char* UnicodeStringView::get_ptr() const {
        return ptr;
    }

    char32_t UnicodeStringView::get_char(std::size_t pos) const {
        return unicode_support::get_symbol(
            ptr + unicode_support::calc_substr_byte_len(ptr, pos)
        );
    }

    std::optional<std::size_t> UnicodeStringView::find(char32_t ch) const {
        return rfind(ch);
    }

    std::optional<std::size_t> UnicodeStringView::rfind(
        char32_t ch
    ) const {
        return unicode_support::rfind_till(ptr, ch, char_len + 1);
    }

    std::optional<std::size_t> UnicodeStringView::rfind_from(
        char32_t ch, std::size_t from
    ) const {
        std::optional<std::size_t> opt = unicode_support::rfind_till(
            ptr + unicode_support::calc_substr_byte_len(ptr, from),
            ch,
            char_len - from + 1
        );
        if (opt.has_value()) {
            return from + opt.value();
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::size_t> UnicodeStringView::rfind_till(
        char32_t ch, std::size_t till
    ) const {
        return unicode_support::rfind_till(
            ptr,
            ch,
            till
        );
    }

    std::optional<std::size_t> UnicodeStringView::rfind_bord(
        char32_t ch, std::size_t from, std::size_t till
    ) const {
        std::optional<std::size_t> opt = unicode_support::rfind_till(
            ptr + unicode_support::calc_substr_byte_len(ptr, from),
            ch,
            till - from + 1
        );
        if (opt.has_value()) {
            return from + opt.value();
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::size_t> UnicodeStringView::lfind(
        char32_t ch
    ) const {
        return unicode_support::lfind_from(ptr, ch, char_len);
    }

    std::optional<std::size_t> UnicodeStringView::lfind_from(
        char32_t ch, std::size_t from
    ) const {
        return unicode_support::lfind_from(ptr, ch, from);
    }

    std::optional<std::size_t> UnicodeStringView::lfind_till(
        char32_t ch, std::size_t till
    ) const {
        std::optional<std::size_t> opt = unicode_support::lfind_from(
            ptr + unicode_support::calc_substr_byte_len(ptr, till),
            ch,
            char_len - till
        );
        if (opt.has_value()) {
            return till + opt.value();
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::size_t> UnicodeStringView::lfind_bord(
        char32_t ch, std::size_t from, std::size_t till
    ) const {
        std::optional<std::size_t> opt = unicode_support::lfind_from(
            ptr + unicode_support::calc_substr_byte_len(ptr, till),
            ch,
            from - till
        );
        if (opt.has_value()) {
            return till + opt.value();
        } else {
            return std::nullopt;
        }
    }

    template <bool CONVERT_TO_LOWER>
    static std::optional<std::size_t> has_substr_base(
        const UnicodeStringView& str,
        const char* substr
    ) {
        if (!substr) {
            return std::nullopt;
        }
        UnicodeStringView uni_substr{substr};
        if (str.get_char_len() == 1) {
            if (uni_substr.get_char_len() == 1
                && uni_substr.get_char(0) == str.get_char(0)
            ) {
                return 0;
            } else {
                return std::nullopt;
            }
        }
        for (auto it = str.cbegin(); it != str.cend(); ++it) {
            bool has_substr = true;
            auto it_copy = it;
            for (
                auto substr_it = uni_substr.cbegin();
                substr_it != uni_substr.cend() && it_copy != str.cend();
                ++substr_it, ++it_copy
            ) {
                bool is_symbols_equals = false;
                if constexpr (CONVERT_TO_LOWER) {
                    char32_t ch1 = 
                        unicode_support::symbol_to_lower(*substr_it);
                    char32_t ch2 = unicode_support::symbol_to_lower(*it);
                    is_symbols_equals = (ch1 == ch2);
                } else {
                    is_symbols_equals = (*substr_it == *it_copy);
                }
                if (!is_symbols_equals) {
                    has_substr = false;
                    break;
                }
            }
            if (has_substr) {
                return it.get_char_pos();
            }
        }
        return std::nullopt;
    }

    std::optional<std::size_t> UnicodeStringView::has_substr(
        const char* substr
    ) const {
        return has_substr_base<false>(*this, substr);
    }

    std::optional<std::size_t> UnicodeStringView::has_substr_lower(
        const char* substr
    ) const {
        return has_substr_base<true>(*this, substr);
    }

    ConstUnicodeStringIter UnicodeStringView::cbegin() const {
        return ConstUnicodeStringIter{ptr, byte_len, 0};
    }

    ConstUnicodeStringIter UnicodeStringView::cend() const {
        return ConstUnicodeStringIter{ptr, byte_len, byte_len};
    }
}

static inline void mem_copy_farward(const char* src,
                                            char* dst, std::size_t len) {
        for (std::size_t q = 0; q < len; ++q) {
            dst[len - q - 1] = src[len - q - 1]; // DEV [optimize?]
        }
    }

static inline void mem_copy_backfarward(const char* src,
                                        char* dst, std::size_t len) {
    for (std::size_t q = 0; q < len; ++q) {
        dst[q] = src[q]; // DEV [optimize?]
    }
}

namespace utils { // UnicodeString
    UnicodeString::UnicodeString()
        : UnicodeStringView()
        , buf_len(0)
    {}

    UnicodeString::UnicodeString(const char* str)
        : UnicodeStringView(str)
    {
        buf_len = byte_len + 1;
        ptr = new char[buf_len];
        mem_copy_farward(str, deconst_ptr(), buf_len);
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
            mem_copy_farward(str.ptr, deconst_ptr(), buf_len);
        } else {
            byte_len = str.byte_len;
            char_len = str.char_len;
            mem_copy_farward(str.ptr, deconst_ptr(), byte_len + 1);
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

    void UnicodeString::insert_symbol_by_char_pos(std::size_t char_pos,
                                                        char32_t symbol) {
        std::size_t symbol_len = unicode_support::symbol_len(symbol);
        std::size_t byte_pos =
                    unicode_support::calc_substr_byte_len(ptr, char_pos);
        if (symbol_len + byte_len + 1 <= buf_len) {
            mem_copy_farward(
                ptr + byte_pos,
                deconst_ptr() + byte_pos + symbol_len,
                byte_len - byte_pos
            );
            unicode_support::paste_symbol(
                deconst_ptr() + byte_pos,
                symbol
            );
        } else {
            buf_len *= 2;
            char* new_ptr = new char[buf_len];
            mem_copy_farward(ptr, new_ptr, byte_pos);
            mem_copy_farward(
                ptr + byte_pos,
                new_ptr + byte_pos + symbol_len,
                byte_len - byte_pos
            );
            unicode_support::paste_symbol(new_ptr + byte_pos, symbol);
            delete std::exchange(ptr, new_ptr);
        }
        byte_len += symbol_len;
        char_len += 1;
        deconst_ptr()[byte_len] = '\0';
    }

    void UnicodeString::delete_symbol_by_char_pos(std::size_t char_pos) {
        std::size_t symbol_len = unicode_support::symbol_len(
            get_char(char_pos)
        );
        std::size_t byte_pos =
                    unicode_support::calc_substr_byte_len(ptr, char_pos);
        mem_copy_backfarward(
            ptr + byte_pos + symbol_len,
            deconst_ptr() + byte_pos,
            byte_len - byte_pos
        );
        byte_len -= symbol_len;
        char_len -= 1;
        deconst_ptr()[byte_len] = '\0';
    }

    std::string_view UnicodeString::to_string_view() const {
        if (ptr) {
            return ptr;
        } else {
            return {};
        }
    }

    // std::optional<std::size_t> UnicodeString::rfind() const {
        
    // }
}
