/**
 * pass
 */

#pragma once

#include <string_view>
#include <string>



namespace utils {
    class UnicodeStringView {
        protected:
            const char* ptr;
            std::size_t byte_len;
            std::size_t char_len;

            static std::size_t calc_byte_len(const char* ptr);
            static std::size_t calc_char_len(const char* ptr,
                                                    std::size_t byte_len);

        public:
            UnicodeStringView();

            UnicodeStringView(const char* ptr);
            UnicodeStringView(std::string_view str);
            UnicodeStringView(const UnicodeStringView&);
            UnicodeStringView& operator=(const UnicodeStringView&);
            UnicodeStringView(UnicodeStringView&&);
            UnicodeStringView& operator=(UnicodeStringView&&);
            ~UnicodeStringView();

            std::size_t get_substr_byte_len(std::size_t
                                                    substr_char_len) const;

            std::size_t get_byte_len() const;
            std::size_t get_char_len() const;

            static std::size_t symbol_len(char32_t c);
            static void paste_symbol(char* ptr, char32_t c);

            void set_new_ptr(const char* ptr);
    };     
}
