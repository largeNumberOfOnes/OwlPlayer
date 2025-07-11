/**
 * Logger.
 */
#pragma once

#include <functional>
#include <iostream>
#include <string>



namespace had {
    class Logger {
        public:
            std::function<void(std::string)> log_err;
            std::function<void(std::string)> log_warn;
            std::function<void(std::string)> log_info;
    };

    static had::Logger stdlogger = {
        [](std::string mes){ std::cerr << "Error: " << mes << std::endl; },
        [](std::string mes){ std::cerr << "Warn : " << mes << std::endl; },
        [](std::string mes){ std::cerr << "Info : " << mes << std::endl; },
    };
    
    static had::Logger black_logger = {
        [](std::string mes){},
        [](std::string mes){},
        [](std::string mes){},
    };
};
