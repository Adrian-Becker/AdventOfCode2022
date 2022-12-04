#include <iostream>
#include "InputManager.h"

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    int count = 0;

    while (input.next_line()) {
        int a0 = input.num("-");
        int a1 = input.num(",");
        int b0 = input.num("-");
        int b1 = input.line_as_num();

        // [a0, a1] is subset of [b0, b1] or [b0, b1] of [a0, a1]
        if ((a0 <= b0 && a1 >= b1) || (b0 <= a0 && b1 >= a1)) ++count;
    }

    std::cout << count << std::endl;
}