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
    //     public:
    //         Res() : val(false);
    // }
    // class Res {
    //     bool val;
    //     public:
    //         static const bool success = true;
    //         static const bool error = false;
    //         Res() : val(false) {}
    //         Res(bool val) : val(val) {}
    //         Res(const Res& other) : val(other.val) {}
    //         operator bool() {
    //             return val;
    //         }
    // };
};
