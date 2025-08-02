#include "fileManager.h"

#include "had/had_types.h"
#include "utils/unicodeString.h"
#include "had/had.h"
#include "setup.h"

#include <cstddef>
#include <string_view>
#include <filesystem>
#include <optional>
#include <utility>
#include <string>
#include <vector>



FileManager::FileManager(
    std::string dir, 
    std::function<void(std::string_view)> call_on_file,
    had::Drawer& drawer,
    const Setup& setup,
    const had::Logger& log
)
    : dir(dir)
    , call_on_file(call_on_file)
    , drawer(drawer)
    , setup(setup)
    , log(log)
{
    if (!this->dir.ends_with('/')) {
        this->dir += '/';
    }
    reload();
}

FileManager::~FileManager() = default;

had::Res FileManager::go() {
    const std::filesystem::path& path = list[selecter].file.path();
    if (list[selecter].file.is_directory()) {
        dir = path;
        dir += '/';
        reload();
    } else if (list[selecter].file.is_regular_file()) {
        if (had::Audio::can_be_played(path.c_str())) {
            call_on_file(path.c_str());
            playing_file = std::move(path.string());
            reload();
        } else {
            log.log_err("File is not playable");
            return had::Res::error;
        }
    } else {
        log.log_err("bad file path: " + path.string());
        return had::Res::error;
    }
    return had::Res::success;
}

had::Res FileManager::release() {
    playing_file = std::nullopt;
    playing_file_pointer = std::nullopt;
    return had::Res::success;
}

had::Res FileManager::back() {
    if (dir != "/") {
        dir.pop_back();
        while (!dir.ends_with('/')) {
            dir.pop_back();
        }
        top = 0;
        selecter = 0;
        reload();
    }
    return had::Res::success;
}

had::Res FileManager::up() {
    if (list_size == 0)
        return had::Res::success;

    if (selecter == 0) {
        selecter = list_size - 1;
        if (size < list_size)
            top = list_size - size;
        else
            top = 0;
    } else {
        --selecter;
        if (selecter + 1 == top)
            --top;
    }

    return had::Res::success;
}

had::Res FileManager::down() {
    if (list_size == 0)
        return had::Res::success;

    if (selecter + 1 == list_size) {
        selecter = 0;
        top = 0;
    } else {
        ++selecter;
        if (selecter >= top + size)
            ++top;
    }

    return had::Res::success;
}

had::Res FileManager::select(had::Dem new_pointer) {
    if (list_size == 0) {
        return had::Res::success;
    }
    if (list_size <= new_pointer) {
        return had::Res::error;
    }
    if (list_size <= size) {
        selecter = new_pointer;
        return had::Res::success;
    }
    if (top <= new_pointer && new_pointer < top + size) {
        selecter = new_pointer;
        return had::Res::success;
    }
    if (new_pointer < size / 2) {
        top = 0;
        selecter = new_pointer;
    } else {
        top = new_pointer - size / 2;
        selecter = new_pointer;
        return had::Res::success;
    }
    return had::Res::error;
}

had::Res FileManager::reload() {
    list.clear();
    namespace fs = std::filesystem;

    // DEV [maybe there should be placed try-catch block]
    for (const auto& entry : fs::directory_iterator(dir)) {
        std::optional<Selection> selection =
                                search_comp(entry.path().stem().c_str());
        if (is_in_search_mode && !selection.has_value()) {
            continue;
        }
        list.push_back(File{
            .reduce_len = std::nullopt,
            .selection  = selection,
            .file       = entry
        });
    }

    std::sort(
        list.begin(), list.end(),
        [](auto a, auto b) {
            if (a.file.is_directory() and b.file.is_directory()
                || a.file.is_regular_file() and b.file.is_regular_file()
            ) {
                return a.file.path() < b.file.path();
            } else if (a.file.is_directory()) {
                return true;
            } else {
                return false;
            }
        }
    );

    // move list if necessary
    list_size = list.size();
    if (list_size == 0) {
        top = 0;
        selecter = 0;
    } else if (selecter >= list_size) {
        selecter = list_size - 1;
        if (list_size > size)
            top = selecter - size;
        else
            top = 0;
    }

    // set if file is playing
    playing_file_pointer = std::nullopt;
    if (playing_file.has_value()) {
        for (int q = 0; q < list_size; ++q) {
            if (list[q].file.path().compare(playing_file.value()) == 0) {
                playing_file_pointer = q;
                break;
            }
        }
    }

    resize();

    return had::Res::success;
}

had::Res FileManager::resize_width() {
    had::Dem w = drawer.get_width();
    for (auto& it : list) {
        utils::UnicodeString uni_str{
            it.file.path().filename().c_str()
        };
        std::size_t char_len = uni_str.get_char_len();
        bool is_reduced = (w < char_len + line_offset + line_free_space);
        if (is_reduced) {
            it.reduce_len = utils::unicode_support::calc_substr_byte_len(
                uni_str.get_ptr(),
                w - line_offset - line_free_space
            );
            log.log_info(std::to_string(it.reduce_len.value()));
            log.log_info(it.file.path().c_str());
        } else {
            it.reduce_len = std::nullopt;
        }
    } 
    return had::Res::success;
}

had::Res FileManager::resize_height() {
    size = drawer.get_height() - 2;
    if (selecter >= top + size) {
        if (size < list_size)
            top = selecter - size + 1;
        else
            top = 0;
    }
    if (list_size < top + size) {
        if (size < list_size)
            top = list_size - size + 1;
        else
            top = 0;
    }
    return had::Res::success;
}

had::Res FileManager::resize() {
    if (drawer.get_width() < min_w || drawer.get_height() < min_h) {
        return had::Res::error;
    }
    if (false
        || resize_width()
        || resize_height()
    ) {
        return had::Res::error;
    }
    return had::Res::success;
}

had::Res FileManager::draw_tree_symbol(int q) {
    had::SpSymbol list_symbol = (top + q + 1 == list_size)
                              ? had::SpSymbol::list_end_symbol
                              : had::SpSymbol::list_symbol;
    if (false
        || drawer.draw_sp_symbol(1, q + 1, list_symbol)
        || drawer.draw_sp_symbol(2, q + 1, had::SpSymbol::list_line_symbol)
    ) {
        log.log_info("Cannot draw_tree_symbol");
        return had::Res::error;
    }
    return had::Res::success;
}

had::Res FileManager::draw_file_name(int q) {
    const File& list_elem = list[top+q];
    bool is_selected  = (top + q == selecter);
    enum class Type {
        directory,
        file,
        playing,
    } type = [&]() {
        if (list_elem.file.is_directory()) {
            return Type::directory;
        } else {
            if (playing_file_pointer.has_value()
                && top + q == playing_file_pointer.value()
            ) {
                return Type::playing;
            } else {
                return Type::file;
            }
        }
    }();
    had::Color file_color = [&]() {
        if (is_selected) {
            switch (type) {
                case Type::directory: return setup.colors.dir_selected;
                case Type::file     : return setup.colors.file_selected;
                case Type::playing  : return setup.colors.playing_selected;
            }
        } else {
            switch (type) {
                case Type::directory: return setup.colors.dir;
                case Type::file     : return setup.colors.file;
                case Type::playing  : return setup.colors.playing;
            }
        }
        return setup.colors.file;
    }();
    drawer.set_color(file_color);
    if (list_elem.reduce_len.has_value()) {
        had::Dem w = drawer.get_width();
        drawer.draw_text_n(
            line_offset,
            q + 1,
            list_elem.file.path().filename().c_str(),
            list_elem.reduce_len.value()
        );
        drawer.draw_text(w - line_free_space, q + 1, "... ");
    } else {
        drawer.draw_text(
            line_offset, q + 1, list_elem.file.path().filename().c_str()
        );
    }
    drawer.set_color(setup.colors.manager_search_select);
    if (list_elem.selection.has_value()) {
        const auto& selection = list_elem.selection.value();
        std::size_t selection_byte_len =
                                selection.till_byte - selection.from_byte;
        if (list_elem.reduce_len.has_value()) {
            if (selection.from_byte < list_elem.reduce_len.value()) {
                selection_byte_len = std::min(
                    selection_byte_len,
                    list_elem.reduce_len.value() - selection.from_byte
                );
            } else {
                selection_byte_len = 0;
            }
        }
        if (0 < selection_byte_len) {
            drawer.draw_text_n(
                line_offset + selection.from_char,
                q + 1,
                list_elem.file.path().filename().c_str()
                                                    + selection.from_byte,
                selection_byte_len
            );
        }
    }
    drawer.set_color(setup.colors.def);
    return had::Res::success;
}

had::Res FileManager::draw_scrol_line() {
    for (int q = 2; q < drawer.get_height() - 1; ++q) {
        drawer.draw_sp_symbol(
            drawer.get_width() - 1, q,
            had::SpSymbol::scrol_middle_symbol
        );
    }
    drawer.draw_sp_symbol(
        drawer.get_width() - 1, 1,
        had::SpSymbol::scrol_up_symbol
    );
    drawer.draw_sp_symbol(
        drawer.get_width() - 1, drawer.get_height() - 1,
        had::SpSymbol::scrol_bottom_symbol
    );

    return had::Res::success;
}

had::Res FileManager::draw_search_line() {
    had::Dem h = drawer.get_height();
    const char* str_ptr = search_str.c_str();
    utils::UnicodeStringView uni_str{str_ptr};
    drawer.draw_symbol(0, h-1, ':');
    if (is_in_search_mode) {
        drawer.draw_text(2, h-1, str_ptr);
        char32_t ch = (search_curs_pos < uni_str.get_char_len())
            ? uni_str.get_char(search_curs_pos)
            : ' ';
        drawer.set_color(setup.colors.manager_search_select);
        drawer.draw_symbol_char32(
            2 + search_curs_pos,
            h - 1,
            ch
        );
        drawer.set_color(setup.colors.def);
    }
    return had::Res::success;
}

had::Res FileManager::draw() {
    had::Dem w = drawer.get_width();
    had::Dem h = drawer.get_height();
    if (w < min_w || h < min_h) {
        log.log_err("bad sizes");
        return had::Res::error;
    }
    static had::Dem w_old = 0;
    static had::Dem h_old = 0;
    if (w != w_old || h != h_old) {
        w_old = w;
        h_old = h;
        resize();
    }
    drawer.cls();
    drawer.draw_text(0, 0, dir.c_str());
    std::size_t files_count = std::min(list_size - top, size);
    for (std::size_t q = 0; q < files_count; ++q) {
        draw_tree_symbol(q);
        draw_file_name(q);
    }
    draw_scrol_line();
    draw_search_line();
    return had::Res::success;
}

std::vector<std::string> FileManager::get_dirs_files() {
    std::vector<std::string> ret;
    for (int q = 0; q < list_size; ++q) {
        const File& it = list[(selecter + q) % list_size];
        if (it.file.is_regular_file()
            && had::Audio::can_be_played(it.file.path().c_str())
        ) {
            ret.push_back(it.file.path().c_str());
        }
    }
    return ret;
}

std::optional<std::string> FileManager::get_selected_comp() {
    if (list[selecter].file.is_regular_file()
        && had::Audio::can_be_played(list[selecter].file.path().c_str())
    ) {
        return list[selecter].file.path();
    } else {
        return std::nullopt;
    }
}

had::Res FileManager::set_playing_file(std::string_view path) {
    std::filesystem::path file_path{path};
    dir = file_path.parent_path();
    reload(); // DEV [ret code]
    for (int q = 0; q < list_size; ++q) {
        if (list[q].file.path() == file_path) {
            selecter = q;
            break;
        }
    }
    return go();
}

std::optional<FileManager::Selection> FileManager::search_comp(
    const char* ptr
) {
    utils::UnicodeStringView uni_str{ptr};
    utils::UnicodeStringView uni_search_str{search_str};
    std::optional<std::size_t> selection_start =
                                uni_str.has_substr(search_str.c_str());
    if (selection_start.has_value()) {
        std::size_t from_char = selection_start.value();
        std::size_t from_byte =
            utils::unicode_support::calc_substr_byte_len(
                ptr,
                selection_start.value()
            );
        return Selection {
            from_char,
            from_byte,
            from_char + uni_search_str.get_char_len(),
            from_byte + uni_search_str.get_byte_len()
        };
    } else {
        return std::nullopt;
    }
}

void FileManager::search_start() {
    is_in_search_mode = true;
}

void FileManager::search_stop() {
    is_in_search_mode = false;
}

void FileManager::search_set_string(std::string_view str, int curs_pos) {
    if (is_in_search_mode) {
        search_str = str;
        search_curs_pos = curs_pos;
        reload();
    }
}

void FileManager::search_clear_string() {
    if (is_in_search_mode) {
        search_str.clear();
        reload();
    }
}
