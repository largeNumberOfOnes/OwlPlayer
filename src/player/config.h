#pragma once

#include "abstract.h"

#include <string>


/** expected config (Only in this order!)
 * default_file = "path_to_file" | "path_to_dir" | None
 * is_hiden = false
 */

class Config {
    private:
        std::string const config_name = ".player_rc";

        std::string start_dir  = "";
        std::string start_file = "";
        bool is_hiden = false;

    public:
        Config(std::string path);
        Config(Config const&) = delete;
        void operator =(Config const&) = delete;
        ~Config();

        stat load();

};
