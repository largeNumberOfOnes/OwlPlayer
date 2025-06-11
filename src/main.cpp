/**
 * This is the main application file from which code execution starts.
 */

#include "had/had.h"
#include "app.h"



int main() {
    App app{had::blacklogger};

    app.run();


    return 0;
}
