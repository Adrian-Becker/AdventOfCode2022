#include <iostream>
#include <array>
#include "InputManager.h"

void draw_cycle(long cycle, long x) {
    if (std::abs(x - (cycle - 1) % 40) <= 1) std::cout << '#';
    else std::cout << '.';
    if (cycle > 0 && cycle % 40 == 0) std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    long x = 1;

    for (long cycle = 1; input.next_line(); ++cycle) {
        if (input.line() == "noop") {
            draw_cycle(cycle, x);
        } else {
            // skip "addx "
            input.skip(5);

            draw_cycle(cycle, x);
            draw_cycle(cycle + 1, x);

            x += input.line_as_num();
            ++cycle;
        }
    }
}