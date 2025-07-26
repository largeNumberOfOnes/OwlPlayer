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

std::optional<had::KeySequence> InputWrapper::get_key() {
    had::KeySequence seq = interface.catch_key_seq();
    
    if (not seq.is_empty()) {
        log.log_info(seq.to_str());
    }

    if (!is_captured) {
        return seq;
    }

    if (seq == exit_seq) {
        instigator.value().last_call(editor.drop());
        release(instigator.value().id);
    } else {
        instigator.value().call(editor.get_string(), editor.get_cursor());
        // pass [process input to editor]
    }

    return std::nullopt;
}
