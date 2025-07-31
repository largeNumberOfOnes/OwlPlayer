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
}

namespace utils {
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

    void UnicodeStringView::set_new_ptr(const char* ptr) {
        this->ptr = ptr;
        byte_len = unicode_support::calc_byte_len(ptr);
        char_len = unicode_support::calc_char_len(ptr);
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
        return ptr;
    }

    // std::optional<std::size_t> UnicodeString::rfind() const {
        
    // }
}
