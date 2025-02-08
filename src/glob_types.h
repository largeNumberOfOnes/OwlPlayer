#pragma once

#include <iostream>



enum stat {
    success = 0,
    error   = 1,
};

inline void log_err(const char* mes) {
    std::cout << mes << std::endl;
}
