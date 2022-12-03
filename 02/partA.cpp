#include <InputManager.h>
#include <iostream>

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    long score = 0;

    while (input.next_line()) {
        // rock = 1, paper = 2, scissors = 3
        int opponent = input.substring(1)[0] - 'A' + 1;
        input.skip(1);
        // rock = 1, paper = 2, scissors = 3
        int self = input.substring(1)[0] - 'X' + 1;

        score += self;
        if (opponent == self) score += 3;
        else if (
                (opponent == 1 && self == 2) ||
                (opponent == 2 && self == 3) ||
                (opponent == 3 && self == 1)
                )
            score += 6;
    }

    std::cout << score << std::endl;
}