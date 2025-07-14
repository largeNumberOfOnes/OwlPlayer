/**
 * pass
 */

#pragma once

#include <string>
#include <string_view>



namespace had {
    class UnicodeStringView {
        const char* ptr;
        std::size_t byte_len;
        std::size_t char_len;

        static std::size_t calc_byte_len(const char* ptr);
        static std::size_t calc_char_len(const char* ptr,
                                                    std::size_t byte_len);

        public:
            UnicodeStringView(const char* ptr);
            UnicodeStringView(std::string_view str);

            std::size_t get_substr_byte_len(std::size_t
                                                    substr_char_len) const;

            std::size_t get_byte_len() const;
            std::size_t get_char_len() const;
    };     
}
