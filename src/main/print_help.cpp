#include <iostream>
#include <fstream>

// #include "../glob.h"
#include "modes.h"

char const* help_mes_path = "doc.txt";

glob::RET print_help() {

    std::ifstream help_file(help_mes_path); 

    if (!help_file.is_open()) {
        std::fprintf(
            stderr,
            "Error cannot open help_file[%s]",
            help_mes_path
        );
        return glob::RET::SUCCES;
    }

    std::string help;
    help_file >> help;
    std::cout << help << std::endl;

    help_file.close();

    return glob::RET::FAILURE;
}
