#ifndef AOC2022_INPUTMANAGER_H
#define AOC2022_INPUTMANAGER_H

#include <string>
#include <sstream>
#include <vector>
#include <fstream>

class InputManager {
public:
    InputManager(int argc, char *argv[]);

    // checks whether there is another line and moves to the next line
    bool next_line();

    // checks whether the current line is empty
    bool empty() const;

    // return the current line
    const std::string &line() const;

    // splits a string by a delimiter D in the form ADB, returns A and deletes AD from the string
    std::string substring(std::string delimiter = " ");

    // returns a substring of length n and removes it from the string
    std::string substring(int n);

    // returns a single char and removes it from the string
    char get_char();

    // skips the next n-chars
    void skip(int n);

    // returns the first number in the current string and removes it
    int num(std::string delimiter = "");

    // returns the first number in the current string and removes it
    double num_double(std::string delimiter = "");

    // parses the current line as a number
    int line_as_num() const;

    // parses the current line as a number
    double line_as_num_double() const;

    // returns the first n-numbers in the current string and removes them
    std::vector<int> nums(std::size_t n, std::string delimiter = "");

    // returns the first n-numbers in the current string and removes them
    std::vector<double> nums_double(std::size_t n, std::string delimiter = "");

    // returns the all numbers in the current string and removes them
    std::vector<int> nums(std::string delimiter = "");

    // returns the all numbers in the current string and removes them
    std::vector<double> nums_double(std::string delimiter = "");

    // returns the number of characters in the current line
    size_t size() const;

    // check if the current line starts with the given prefix
    bool starts_with(const std::string &prefix) const;
private:
    std::ifstream input;
    std::string current_line;
};


#endif //AOC2022_INPUTMANAGER_H
