
#include <InputManager.h>
#include <iostream>

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    long score = 0;

    while (input.next_line()) {
        // rock = 1, paper = 2, scissors = 3
        int opponent = input.substring(1)[0] - 'A' + 1;
        input.skip(1);
        // loose = 1, draw = 2, win = 3
        int self = input.substring(1)[0] - 'X' + 1;

        if (self == 1) {
            if (opponent == 1) score += 3;
            else if (opponent == 2) score += 1;
            else score += 2;
        } else if (self == 2) {
            score += opponent;
            score += 3;
        } else {
            if (opponent == 1) score += 2;
            else if (opponent == 2) score += 3;
            else score += 1;
            score += 6;
        }
    }

    std::cout << score << std::endl;
}