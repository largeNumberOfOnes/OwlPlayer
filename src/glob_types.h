#pragma once

#include <iostream>



enum res {
    success = 0,
    error   = 1,
};

#ifdef BUILD_DEB

#define log_err(...) {                                                    \
    printf("\033[91mError: %s, %d: \033[39m", __FILE__, __LINE__);        \
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

#define TEST(test) {                                                      \
    test();                                                               \
    std::cout << "[OK] " << #test << std::endl;                           \
}