#pragma once

#include <iostream>



enum res {
    success = 0,
    error   = 1,
};

#ifdef BUILD_DEB

#define log_err(...) {                                                       \
    printf(__VA_ARGS__);                                        \
    printf("\n");                                        \
}                                                                         \

#else

#define log_err(...)

#endif // BUILD_DEB
