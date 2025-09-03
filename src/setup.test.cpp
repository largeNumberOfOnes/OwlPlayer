#include "setup.h"



void parse_string(std::string str) {
    // set <parm> <val>
    // map <sequense> <action>

    const auto END = str.cend();

    if (str.length() == 0) {
        // DEV [bad str]
    }

    auto it = str.cbegin();

    std::string mod = "";
    while (true) {
        if (it == END || *it == ' ') {
            break;
        }
        mod.push_back(*it);
        ++it;
    }

    if (mod == "set") {
        // pass
    } else if (mod == "map") {
        // pass
    } else {
        // pass
    }

    // row until next word
    while (true) {
        if (it == END) {
            // pass
        }
        if (*it != ' ') {
            break;
        }
        ++it;
    }

    
}

void read_() {
    parse_string("s");
}

int main() {

    // Setup setup;

    // setup.read_config("test_data/test_config.cong");

    read_();

    return 0;
}
