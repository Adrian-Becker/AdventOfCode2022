#include "LineInput.h"

#include <utility>

LineInput::LineInput(std::string string) : current_line(std::move(string)) {}

std::size_t LineInput::size() const {
    return current_line.size();
}

bool LineInput::empty() const {
    return current_line.empty();
}

const std::string &LineInput::line() const {
    return current_line;
}

std::string LineInput::substring(std::string delimiter) {
    auto index = current_line.find(delimiter);
    if (index == std::string::npos) {
        std::string string = current_line;
        current_line.clear();
        return string;
    }
    std::string string = current_line.substr(0, index);
    current_line.erase(0, index + delimiter.length());
    return string;
}

int LineInput::num(std::string delimiter) {
    return std::stoi(substring(delimiter));
}

double LineInput::num_double(std::string delimiter) {
    return std::stod(substring(delimiter));
}

std::vector<int> LineInput::nums(std::size_t n, std::string delimiter) {
    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) values.push_back(num(delimiter));
    return values;
}

std::vector<double> LineInput::nums_double(std::size_t n, std::string delimiter) {
    std::vector<double> values(n);
    for (int i = 0; i < n; ++i) values.push_back(num_double(delimiter));
    return values;
}

std::vector<int> LineInput::nums(std::string delimiter) {
    std::vector<int> values;
    while (!empty()) {
        values.push_back(num(delimiter));
    }
    return values;
}

std::vector<double> LineInput::nums_double(std::string delimiter) {
    std::vector<double> values;
    while (!empty()) {
        values.push_back(num_double(delimiter));
    }
    return values;
}

int LineInput::line_as_num() const {
    return std::stoi(current_line);
}

double LineInput::line_as_num_double() const {
    return std::stod(current_line);
}

std::string LineInput::substring(int n) {
    std::string result = current_line.substr(0, n);
    current_line.erase(0, n);
    return result;
}

void LineInput::skip(int n) {
    if (n >= 0) current_line.erase(0, std::min(n, (int) current_line.length()));
    else current_line.erase(std::max(0, (int) current_line.length() + n), current_line.length());
}

char LineInput::get_char() {
    char c = current_line[0];
    current_line.erase(0, 1);
    return c;
}

bool LineInput::starts_with(const std::string &prefix) const {
    return current_line.rfind(prefix, 0) == 0;
}

bool LineInput::contains(const std::string &string) {
    auto index = current_line.find(string);
    return index != std::string::npos;
}
