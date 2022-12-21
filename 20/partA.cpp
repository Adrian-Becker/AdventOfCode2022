#include <algorithm>
#include <iostream>
#include <list>
#include "InputManager.h"

void shift(std::list<int> &numbers, std::list<int>::iterator node) {
    int value = *node;
    if (value == 0) return;

    if (value > 0) {
        auto it = node;
        ++it;
        if (it == numbers.end()) it = numbers.begin();
        numbers.erase(node);
        for (int i = 0; i < value; ++i) {
            ++it;
            if (it == numbers.end()) it = numbers.begin();
        }
        numbers.insert(it, value);
    } else {
        auto it = node;
        --it;
        if (it == numbers.begin()) it = numbers.end();
        numbers.erase(node);
        for (int i = 0; i < -value - 1; ++i) {
            if (it == numbers.begin()) it = numbers.end();
            --it;
        }
        if (it == numbers.begin()) it = numbers.end();
        numbers.insert(it, value);
    }
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    std::list<int> numbers;
    std::list<std::list<int>::iterator> elements;

    while (input.next_line()) {
        numbers.push_back(input.line_as_num());
        elements.push_back(--numbers.end());
    }

    std::cout << "Shifting numbers" << std::endl;
    for (auto it: elements) {
        shift(numbers, it);
        // only print list if output is reasonably small
        if (numbers.size() < 20) {
            std::cout << std::endl;
            for (auto n: numbers) std::cout << n << ' ';
            std::cout << std::endl;
        }
    }


    std::cout << std::endl << "Searching the number 0" << std::endl;
    auto it = numbers.begin();
    for (; *it != 0; ++it);

    int result = 0;
    for (int i = 1; i <= 3; ++i) {
        // move 1000 positions right
        for (int j = 0; j < 1000 % numbers.size(); ++j) {
            ++it;
            if (it == numbers.end()) it = numbers.begin();
        }
        // add number
        std::cout << i << " coordinate is " << *it << std::endl;
        result += *it;
    }

    std::cout << "Sum of coordinates is " << result << std::endl;
}
