/**
 * pass
 */

#pragma once

#include <cstddef>
#include <optional>
#include <string_view>



namespace utils::unicode_support {
    std::size_t calc_byte_len(const char* ptr);
    std::size_t calc_char_len(const char* ptr);
    std::size_t symbol_len(char32_t c);
    void paste_symbol(char* ptr, char32_t c);
    std::size_t calc_substr_byte_len(const char* ptr,
                                            std::size_t substr_char_len);

    std::optional<std::size_t> rfind_till(const char* ptr, char32_t ch,
                                                    std::size_t till_char);
    std::optional<std::size_t> lfind_from(const char* ptr, char32_t ch,
                                                    std::size_t from_char);
}

namespace utils {
    class UnicodeStringView {
        protected:
            const char* ptr;
            std::size_t byte_len;
            std::size_t char_len;

        public:
            UnicodeStringView();
            UnicodeStringView(const char* ptr);
            UnicodeStringView(std::string_view str);
            UnicodeStringView(const UnicodeStringView&);
            UnicodeStringView& operator=(const UnicodeStringView&);
            UnicodeStringView(UnicodeStringView&&);
            UnicodeStringView& operator=(UnicodeStringView&&);
            ~UnicodeStringView();

            std::size_t get_byte_len() const;
            std::size_t get_char_len() const;
            char32_t get_char(std::size_t pos) const;

            std::optional<std::size_t> find(char32_t ch) const;

            std::optional<std::size_t> rfind(
                char32_t ch
            ) const;
            std::optional<std::size_t> rfind_from(
                char32_t ch, std::size_t from
            ) const;
            std::optional<std::size_t> rfind_till(
                char32_t ch, std::size_t till
            ) const;
            std::optional<std::size_t> rfind_bord(
                char32_t ch, std::size_t from, std::size_t till
            ) const;

            std::optional<std::size_t> lfind(
                char32_t ch
            ) const;
            std::optional<std::size_t> lfind_from(
                char32_t ch, std::size_t from
            ) const;
            std::optional<std::size_t> lfind_till(
                char32_t ch, std::size_t till
            ) const;
            std::optional<std::size_t> lfind_bord(
                char32_t ch, std::size_t from, std::size_t till
            ) const;

            void set_new_ptr(const char* ptr);
    }; 

    class UnicodeString : public UnicodeStringView {
        std::size_t buf_len = 0;

        constexpr inline char* deconst_ptr() {
            return const_cast<char*>(ptr);
        }

        inline void mem_copy_farward(const char* src, char* dst,
                                                        std::size_t len);
        inline void mem_copy_backfarward(const char* src, char* dst,
                                                        std::size_t len);

        public:
            class Iter {
                const UnicodeStringView& ref;

                Iter(const UnicodeStringView& ref) : ref(ref) {}
            };

        public:
            UnicodeString();

            UnicodeString(const char* str);
            UnicodeString(std::string_view str);

            UnicodeString(const UnicodeString& str);
            UnicodeString& operator=(const UnicodeString& str);
            UnicodeString(UnicodeString&& str);
            UnicodeString& operator=(UnicodeString&& str);
            
            ~UnicodeString();

            void insert_symbol_by_char_pos(std::size_t char_pos,
                                                        char32_t symbol);
            void delete_symbol_by_char_pos(std::size_t char_pos);

            std::string_view to_string_view() const;

            Iter get_iter();
    };
}
