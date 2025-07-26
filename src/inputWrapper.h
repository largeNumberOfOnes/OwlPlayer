/**
 * pass
 */

#pragma once

#include "had/had.h"
#include "had/had_interface.h"
#include "had/had_keys.h"
#include "stringEditor.h"

#include <string_view>
#include <functional>
#include <optional>



class InputWrapper {
    had::Interface& interface;
    const had::Logger& log;
    bool is_captured = false;
    StringEditor editor;
    had::KeySequence exit_seq;

    public:
        using CaptureId = int;
        using InstigatorCall =
            std::function<void(std::string_view str, had::Dem curs_pos)>;
        using InstigatorLastCall =
            std::function<void(std::string&& str)>;
    
    private:
        struct Instigator {
            InstigatorCall call;
            InstigatorLastCall last_call;
            CaptureId id;
        };
        std::optional<Instigator> instigator;

        CaptureId get_new_id();
        void make_instigator_empty();

    public:
        InputWrapper(had::Interface& interface, had::KeySequence exit_seq,
                                                const had::Logger& log);
        InputWrapper(InputWrapper&)            = delete;
        InputWrapper& operator=(InputWrapper&) = delete;
        ~InputWrapper();

        std::optional<CaptureId> capture(
            InstigatorCall call,
            InstigatorLastCall last_call
        );
        std::optional<CaptureId> capture_with_data(
            InstigatorCall call,
            InstigatorLastCall last_call,
            std::string_view str,
            int curs_pos
        );
        had::Res release(CaptureId id);

        std::optional<had::KeySequence> get_key();
};
