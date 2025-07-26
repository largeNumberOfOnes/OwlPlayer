/**
 * pass
 */

#pragma once

#include "unicodeStringView.h"

#include <string_view>



namespace utils {
    class UnicodeString : public UnicodeStringView {
        std::size_t buf_len = 0;

        constexpr inline char* deconst_ptr() {
            return const_cast<char*>(ptr);
        }

        inline void mem_copy(const char* src, char* dst, std::size_t len);

        public:
            UnicodeString();

            UnicodeString(const char* str);
            UnicodeString(std::string_view str);

            UnicodeString(const UnicodeString& str);
            UnicodeString& operator=(const UnicodeString& str);
            UnicodeString(UnicodeString&& str);
            UnicodeString& operator=(UnicodeString&& str);
            
            ~UnicodeString();

            void insert_symbol_by_char_pos(std::size_t char_posm,
                                                        char32_t symbol);

            std::string_view to_string_view() const;
    };
}
