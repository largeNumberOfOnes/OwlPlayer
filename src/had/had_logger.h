/**
 * Logger.
 */
#pragma once

#include <string_view>
#include <functional>
#include <iostream>
#include <string>



namespace had {
    class Logger {
        public:
            std::function<void(std::string_view)> log_err;
            std::function<void(std::string_view)> log_warn;
            std::function<void(std::string_view)> log_info;
    };

    static had::Logger std_slogger = {
        [](std::string_view mes){ std::cerr << "Error: " << mes << std::endl; },
        [](std::string_view mes){ std::cerr << "Warn : " << mes << std::endl; },
        [](std::string_view mes){ std::cerr << "Info : " << mes << std::endl; },
    };
    
    static had::Logger black_logger = {
        [](std::string_view mes){},
        [](std::string_view mes){},
        [](std::string_view mes){},
    };
};
