#include <InputManager.h>
#include <iostream>
#include <array>

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    long current_calories = 0;

    // the following code is written such that it also allows to record the
    // top n-counts of calories
    std::array<long, 3> top_calories{};
    for (long& value : top_calories) value = 0;

    while (input.next_line()) {
        if (input.empty()) {
            // an empty line means that we reached the end of the current inventory
            // we can now check if it contains more calories
            for (std::size_t i = 0; i < top_calories.size(); ++i) {
                if (current_calories > top_calories[i]) {
                    // insert current value and shift all other values
                    long tmp;
                    for (std::size_t j = i; j < top_calories.size(); ++j) {
                        tmp = top_calories[j];
                        top_calories[j] = current_calories;
                        current_calories = tmp;
                    }
                }
            }
            current_calories = 0;
        } else {
            // increase calories by the calories of the current item
            current_calories += input.line_as_num();
        }
    }
    // check for last elf
    for (std::size_t i = 0; i < top_calories.size(); ++i) {
        if (current_calories > top_calories[i]) {
            // insert current value and shift all other values
            long tmp;
            for (std::size_t j = i; j < top_calories.size(); ++j) {
                tmp = top_calories[j];
                top_calories[j] = current_calories;
                current_calories = tmp;
            }
        }
    }

    long sum_calories = 0.0;
    for (long value : top_calories) {
        sum_calories += value;
    }
    std::cout << sum_calories << std::endl;
}