

#pragma once

#include <unordered_map>
#include <string>



namespace had {

    enum struct Key {
        arrow_rigth,
        arrow_left,
        arrow_up,
        arrow_down,
        space,
        q,
    };

    // static std::unordered_map<std::string, Key> key_map {
    //     std::pair("q", Key::q)
    // };
};
