/*
 * The class represents the globular state of the application.
 */

#pragma once

/** expected config (Only in this order!)
 * default_file = "path_to_file" | "path_to_dir" | None
 * is_hiden = false
 */

class Config {
    public:
        Config(int argc, char const** argv);
        Config(Config const&) = delete;
        Config(Config const&&) = delete;
        void operator=(Config const&) = delete;
        void operator=(Config const&&) = delete;
        ~Config();

        bool run_GUIPlayer();

        char const* get_path();
};
