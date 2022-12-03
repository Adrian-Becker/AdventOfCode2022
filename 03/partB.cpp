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
        // total number of item types a group carries
        std::array<int, 52> items{};
        items.fill(0);

        // process 3 elves
        for (int j = 0; j < 3; ++j) {
            if (j != 0) input.next_line();

            std::array<bool, 52> found{};
            found.fill(false);
            int n = input.size();
            // add items of one elve
            for (int i = 0; i < n; ++i) {
                int object = object_to_number(input.get_char());
                // ensure that each item-type is only counted once
                if (!found[object - 1]) {
                    ++items[object - 1];
                    found[object - 1] = true;
                }
            }
        }

        // check for an item that the group shares
        for (int i = 0; i < items.size(); ++i) {
            if (items[i] == 3) {
                priority += i + 1;
                break;
            }
        }
    }

    std::cout << priority << std::endl;
}