#include "inputWrapper.h"
#include "had/had_keys.h"
#include "had/had_types.h"
#include <optional>



InputWrapper::InputWrapper(had::Interface& interface,
                        had::KeySequence exit_seq, const had::Logger& log)
    : interface(interface)
    , exit_seq(exit_seq)
    , log(log)
{}

InputWrapper::~InputWrapper() = default;

InputWrapper::CaptureId InputWrapper::get_new_id() {
    static CaptureId new_id = 0;
    return new_id++;
}

std::optional<InputWrapper::CaptureId> InputWrapper::capture(
    InstigatorCall call,
    InstigatorLastCall last_call
) {
    if (is_captured) {
        return std::nullopt;
    }
    is_captured = true;
    instigator = Instigator {
        .call = call,
        .last_call = last_call,
        .id = get_new_id(),
    };
    return instigator.value().id;
}

std::optional<InputWrapper::CaptureId> InputWrapper::capture_with_data(
    InstigatorCall call,
    InstigatorLastCall last_call,
    std::string_view str,
    int curs_pos
) {
    if (is_captured) {
        return std::nullopt;
    }
    editor.set(str, curs_pos);
    return capture(call, last_call);
}

had::Res InputWrapper::release(CaptureId id) {
    if (id != instigator->id) {
        return had::Res::error;
    }
    is_captured = false;
    instigator = std::nullopt;
    return had::Res::success;
}

void InputWrapper::editor_proc(had::KeySequence seq) {
    if (seq == had::KeySequence{had::Key::arrow_rigth}) {
        editor.move_cursor_right();
    } else if (seq == had::KeySequence{had::Key::arrow_left}) {
        editor.move_cursor_left();
    } else if (seq == had::KeySequence{had::Key::arrow_rigth}.add_ctrl()) {
        editor.move_on_word_right();
    } else if (seq == had::KeySequence{had::Key::arrow_left}.add_ctrl()) {
        editor.move_on_word_left();
    } else if (seq == had::KeySequence{had::Key::backspace}) {
        editor.delete_symbol_back();
    } else if (seq == had::KeySequence{had::Key::del}) {
        editor.delete_symbol_front();
    } else if (seq.is_alpha()) {
        editor.insert_symbol(seq.to_char());
    }
}

std::optional<had::KeySequence> InputWrapper::get_key() {
    had::KeySequence seq = interface.catch_key_seq();
    
    if (seq.is_empty()) {
        return std::nullopt;
    }
    log.log_info(seq.to_str());

    if (!is_captured) {
        return seq;
    }

    if (seq == exit_seq) {
        log.log_info(std::to_string(instigator.has_value()));
        // instigator.value().last_call(std::string{editor.get_string()});
        release(instigator.value().id);
    } else {
        editor_proc(seq);
        instigator.value().call(editor.get_string(), editor.get_cursor());
        // pass [process input to editor]
    }

    return std::nullopt;
}
