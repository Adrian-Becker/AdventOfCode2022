#include "InputManager.h"

#include <iostream>

InputManager::InputManager(int argc, char **argv) {
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

std::size_t InputManager::size() const {
    return current_line.size();
}

bool InputManager::next_line() {
    if (input.peek() != EOF) {
        std::getline(input, current_line);
        return true;
    }
    return false;
}

bool InputManager::empty() const {
    return current_line.empty();
}

const std::string &InputManager::line() const {
    return current_line;
}

std::string InputManager::substring(std::string delimiter) {
    auto index = current_line.find(delimiter);
    std::string string = current_line.substr(0, index);
    current_line.erase(0, index + delimiter.length());
    return string;
}

int InputManager::num(std::string delimiter) {
    return std::stoi(substring(delimiter));
}

double InputManager::num_double(std::string delimiter) {
    return std::stod(substring(delimiter));
}

std::vector<int> InputManager::nums(std::size_t n, std::string delimiter) {
    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) values.push_back(num(delimiter));
    return values;
}

std::vector<double> InputManager::nums_double(std::size_t n, std::string delimiter) {
    std::vector<double> values(n);
    for (int i = 0; i < n; ++i) values.push_back(num_double(delimiter));
    return values;
}

std::vector<int> InputManager::nums(std::string delimiter) {
    std::vector<int> values;
    while (!empty()) {
        values.push_back(num(delimiter));
    }
    return values;
}

std::vector<double> InputManager::nums_double(std::string delimiter) {
    std::vector<double> values;
    while (!empty()) {
        values.push_back(num_double(delimiter));
    }
    return values;
}

int InputManager::line_as_num() const {
    return std::stoi(current_line);
}

double InputManager::line_as_num_double() const {
    return std::stod(current_line);
}

std::string InputManager::substring(int n) {
    std::string result = current_line.substr(0, n);
    current_line.erase(0, n);
    return result;
}

void InputManager::skip(int n) {
    current_line.erase(0, n);
}

char InputManager::get_char() {
    char c = current_line[0];
    current_line.erase(0, 1);
    return c;
}


