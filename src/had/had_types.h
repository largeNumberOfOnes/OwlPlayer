/**
 * Base types.
 */

#pragma once

#include <cstdint>



namespace had {
    using Dem     = int;
    using seconds = int;
    using Volume  = int;

    using SampleDem = std::size_t;
    using Value = uint16_t;
    static_assert(sizeof(Value) == 2);

    enum Res {
        success = 0,
        error   = 1,
    };
    // class Res {
    //     bool val;
    //     static constexpr bool val_success = false;
    //     static constexpr bool val_error = true;

    //     class ResType {
    //         bool val;
    //         constexpr ResType(bool val) : val(val) {}
    //         friend Res;
    //     };
    //     public:
    //         Res() : val(val_error) {};
    //         Res(ResType res_type) : val(res_type.val) {};
    //         static const ResType success;
    //         static const ResType error;

    //         bool was_success() {
    //             return val == val_success;
    //         }

    //         // operator bool() {
    //         //     return val;
    //         // }
    //         bool operator ==(Res other) {
    //             return val == other.val;
    //         }
    //         Res operator &(Res other) {
    //             if (val == val_success && other.val == val_success) {
    //                 return success;
    //             } else {
    //                 return error;
    //             }
    //         }
    // };

    // inline const Res::ResType Res::success{Res::val_success};
    // inline const Res::ResType Res::error{Res::val_error};
};
