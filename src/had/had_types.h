/**
 * Base types.
 */

#pragma once



namespace had {
    using Dem     = int;
    using seconds = int;
    using Volume  = int;

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

    //         operator bool() {
    //             return val;
    //         }
    //         bool operator ==(Res other) {
    //             return val == other.val;
    //         }
    //         // Res operator ||(Res other) {
    //         //     if (val == val_success && other.val == val_success) {
    //         //         return success;
    //         //     } else {
    //         //         return error;
    //         //     }
    //         // }
    // };

    // inline const Res::ResType Res::success{Res::val_success};
    // inline const Res::ResType Res::error{Res::val_error};
};
