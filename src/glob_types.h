#pragma once

#include <iostream>



enum res {
    success = 0,
    error   = 1,
};

#ifdef BUILD_DEB

#define log_err(...) {                                                    \
    printf("\033[91mError: \033[39m");                                    \
    printf(__VA_ARGS__);                                                  \
    printf("\n");                                                         \
}
#define log_step(...) {                                                   \
    printf("\033[96mStep: \033[39m");                                     \
    printf(__VA_ARGS__);                                                  \
    printf("\n");                                                         \
}

#else

#define log_err(...)
#define log_step(...)

#endif // BUILD_DEB
