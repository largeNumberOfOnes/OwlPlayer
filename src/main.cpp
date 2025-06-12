/**
 * This is the main application file from which code execution starts.
 */

#include "had/had.h"
#include "setup.h"
#include "app.h"

#include <iomanip>

int main() {

    had::Logger log = had::blacklogger;

    Setup setup;

    had::Interface interface{log};
    // interface.create_color(setup.colors.def ,          255, 255, 255, 0, 0, 0);
    interface.get_defult_color(setup.colors.def);
    short dtr, dtg, dtb, dbr, dbg, dbb;
    had::Color def_col;
    interface.get_defult_color(def_col);
    interface.get_color_comp(def_col, dtr, dtg, dtb, dbr, dbg, dbb);
    // std::cout
    //     << dtr << " "
    //     << dtg << " "
    //     << dtb << " "
    //     << dbr << " "
    //     << dbg << " "
    //     << dbb << " "
    //     << std::endl;

    // interface.create_color(setup.colors.def ,          100, 100, 100, dbr, dbg, dbb);
    interface.create_color(setup.colors.def ,          dtr, dtg, dtb, dbr, dbg, dbb);
    interface.create_color(setup.colors.dir ,           21, 118, 230, 0, 0, 0);
    interface.create_color(setup.colors.file,          240, 240, 240, 0, 0, 0);
    interface.create_color(setup.colors.dir_selected,   21, 118, 230, 100, 100, 100);
    interface.create_color(setup.colors.file_selected, 240, 240, 240, 100, 100, 100);
    

    App app{interface, setup, log};

    // while (true) {}
    app.run();


    return 0;
}
