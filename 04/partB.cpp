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

        // checks if a0 or a1 lie in [b0, b1] or if b0 or b1 lie in [a0, a1]
        // iff this is true, there is overlap between the two intervals
        if ((a0 >= b0 && a0 <= b1) || (b0 >= a0 && b0 <= a1) || (a1 >= b0 && a1 <= b1) || (b1 >= a0 && b1 <= a1)) ++count;
    }

    std::cout << count << std::endl;
}