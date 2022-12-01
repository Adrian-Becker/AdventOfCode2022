#include <InputManager.h>
#include <iostream>

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    long max_calories = 0;
    long current_calories = 0;

    while (input.next_line()) {
        if (input.empty()) {
            // an empty line means that we reached the end of the current inventory
            // we can now check if it contains more calories
            if (current_calories > max_calories) max_calories = current_calories;
            current_calories = 0;
        } else {
            // increase calories by the calories of the current item
            current_calories += input.line_as_num();
        }
    }
    // check for last elf
    if (current_calories > max_calories) max_calories = current_calories;

    std::cout << max_calories << std::endl;
}