
#include <InputManager.h>
#include <iostream>
#include <array>

int object_to_number(char raw_object) {
    if (raw_object > 96) {
        // [a..z] gets mapped to [1..26]
        return raw_object - 96;
    } else {
        // [A..Z] gets mapped to [27..52]
        return raw_object - 64 + 26;
    }
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    long priority = 0;

    while (input.next_line()) {
        std::array<bool, 52> first_compartment{};
        first_compartment.fill(false);
        int n = input.size() / 2;
        // add objects of the first compartment
        for (int i = 0; i < n; ++i) {
            int object = object_to_number(input.get_char());
            // mark that the first compartment contains the object
            first_compartment[object - 1] = true;
        }

        // check object of the second compartment
        for (int i = 0; i < n; ++i) {
            int object = object_to_number(input.get_char());
            if (first_compartment[object - 1]) {
                // item is also in first compartment, we can stop here since there is only
                // one error per rucksack
                priority += object;
                break;
            }
        }
    }

    std::cout << priority << std::endl;
}