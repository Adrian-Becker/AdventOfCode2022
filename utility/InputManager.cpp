#include "InputManager.h"

#include <iostream>

InputManager::InputManager(int argc, char **argv) : LineInput("") {
    if (argc < 2) {
        std::cerr << "Second argument must be path to the input file!" << std::endl;
        std::exit(1);
    }
    input = std::ifstream(argv[1], std::ifstream::in);
    if (input.fail()) {
        std::cerr << "The file " << argv[1] << " does not exist!" << std::endl;
        std::exit(1);
    }
}

bool InputManager::next_line() {
    if (input.peek() != EOF) {
        std::getline(input, current_line);
        return true;
    }
    return false;
}


