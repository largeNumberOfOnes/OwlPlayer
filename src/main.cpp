/**
 * This is the main application file from which code execution starts.
 */

#include "had/had.h"
#include "setup.h"
#include "app.h"



int main() {

    had::Logger log = had::blacklogger;

    Setup setup;

    had::Interface interface{log};
    interface.create_color(setup.colors.def ,          255, 255, 255, 0, 0, 0);
    interface.create_color(setup.colors.dir ,           21, 118, 230, 0, 0, 0);
    interface.create_color(setup.colors.file,          240, 240, 240, 0, 0, 0);
    interface.create_color(setup.colors.dir_selected,   21, 118, 230, 100, 100, 100);
    interface.create_color(setup.colors.file_selected, 240, 240, 240, 100, 100, 100);
    
    
    App app{interface, setup, log};

    app.run();


    return 0;
}
