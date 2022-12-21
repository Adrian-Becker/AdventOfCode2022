#include <algorithm>
#include <iostream>
#include <list>
#include "InputManager.h"

void
shift(std::list<long long> &numbers, std::list<long long>::iterator node,
      std::list<std::list<long long>::iterator> &new_iterators) {
    long long value = *node;
    long long move = value % (long long) (numbers.size() - 1);
    move += (long long) numbers.size() - 1;
    move %= (long long) numbers.size() - 1;
    if (move == 0) return;

    auto it = node;
    ++it;
    numbers.erase(node);
    for (int i = 0; i < move; ++i) {
        if (it == numbers.end()) it = numbers.begin();
        ++it;
    }
    new_iterators.push_back(numbers.insert(it, value));
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    std::list<long long> numbers;
    std::list<std::list<long long>::iterator> elements;

    long long decryption_key = 811589153;

    while (input.next_line()) {
        long long num = input.line_as_num();
        numbers.push_back(num * decryption_key);
        elements.push_back(--numbers.end());
    }
    // only print list if output is reasonably small
    if (numbers.size() < 20) {
        std::cout << std::endl;
        for (auto n: numbers) std::cout << n << ' ';
        std::cout << std::endl;
    }

    std::cout << "Shifting numbers" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << "Performing shift number " << (i + 1) << std::endl;
        std::list<std::list<long long>::iterator> new_elements;
        for (auto it: elements) {
            shift(numbers, it, new_elements);
        }
        elements = new_elements;
        new_elements.clear();

        // only print list if output is reasonably small
        if (numbers.size() < 20) {
            for (auto n: numbers) std::cout << n << ' ';
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }


    std::cout << std::endl << "Searching the number 0" << std::endl;
    auto it = numbers.begin();
    for (; *it != 0; ++it);

    std::cout << std::endl;
    long long result = 0;
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
