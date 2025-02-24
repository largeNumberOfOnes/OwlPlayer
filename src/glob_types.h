#pragma once

#include <iostream>



enum res {
    success = 0,
    error   = 1,
};

#ifdef BUILD_DEB

#define log_err() {                                                       \
    std::cerr << mes << std::endl;                                        \
}                                                                         \

#else

#define log_err()

#endif // BUILD_DEB
