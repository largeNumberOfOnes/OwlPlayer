#pragma once

#include <iostream>
#include <iomanip>
#include <thread>


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


inline void deb_waiting_for(
    int time_seconds,
    char const* mes = "",
    int step_count = 100
) {
    std::cout << "waiting [" << mes << "]:   0%" << std::flush;
    for (int q = 0; q < step_count; ++q) {
        std::this_thread::sleep_for(
                std::chrono::milliseconds(time_seconds*1000/step_count));
        std::cout << "\b\b\b\b" << std::setw(3) << 100/step_count*q << "%"
                                                            << std::flush;
    }
    std::cout << std::endl;
}
