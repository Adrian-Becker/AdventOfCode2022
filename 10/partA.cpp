#include <iostream>
#include <array>
#include "InputManager.h"

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    long x = 1;
    long interesting_values = 0;

    for (long cycle = 1; input.next_line(); ++cycle) {
        if (input.line() == "noop") {
            // check if the current cycle is interesting
            if (cycle >= 20 && (cycle - 20) % 40 == 0) {
                interesting_values += cycle * x;
            }
        } else {
            // skip "addx "
            input.skip(5);
            // check if the current or next cycle is interesting
            if (cycle >= 19 && (cycle - 19) % 40 <= 1) {
                interesting_values += x * (cycle % 2 == 0 ? cycle : (cycle + 1));
            }
            x += input.line_as_num();
            ++cycle;
        }
    }

    std::cout << interesting_values << std::endl;
}