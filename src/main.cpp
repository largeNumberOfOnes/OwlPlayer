/**
 * This is the main application file from which code execution starts.
 */

#include "had/had.h"
#include "setup.h"
#include "app.h"

#include <fstream>
#include <iomanip>
#include <string>
#include <string_view>

int main() {

    // had::Logger log = had::black_logger;
    std::string log_file_path =
        "/home/dt/Documents/audioPlayer/logs/log.txt";
    std::ofstream log_file(log_file_path, std::ios::app);
    log_file << "----------------------------- start new log" << std::endl;
    if (!log_file.is_open()) {
        std::cerr << "Error cannot open log file" << std::endl;
        return -1;
    }
    had::Logger log = {
        [&log_file](std::string_view mes)
            { log_file << "Error: " << mes << std::endl; },
        [&log_file](std::string_view mes)
            { log_file << "Warn : " << mes << std::endl; },
        [&log_file](std::string_view mes)
            { log_file << "Info : " << mes << std::endl; },
    };

    Setup setup;

    had::Interface interface{log};
    interface.get_default_color(setup.colors.def);
    interface.create_text_color(setup.colors.dir ,           21, 118, 230);
    interface.create_text_color(setup.colors.file,          240, 240, 240);
    interface.create_color(setup.colors.dir_selected,   21, 118, 230, 100, 100, 100);
    interface.create_color(setup.colors.file_selected, 240, 240, 240, 100, 100, 100);
    // interface.create_text_color(setup.colors.error_str, 237, 55, 9);
    interface.create_color(setup.colors.error_str, 250, 250, 250, 168, 85, 64);
    

    App app{interface, setup, log};

    app.run();

    return 0;
}
