#include <iostream>
#include <array>
#include "InputManager.h"

// check that all characters in the distory are different from each other
template<typename array>
bool is_marker(const array &history) {
    // inefficient, sorting would be faster
    for (int i = 0; i < history.size() - 1; ++i) {
        for (int j = i + 1; j < history.size(); ++j) {
            if (history[i] == history[j]) return false;
        }
    }
    return true;
}

template<typename array>
void shift_array(array &history) {
    for (int i = 0; i < history.size() - 1; ++i) {
        history[i] = history[i + 1];
    }
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);
    input.next_line();

    std::array<char, 14> history{' ', ' ', ' ', ' '};
    for (int i = 0; !input.empty(); ++i) {
        shift_array(history);

        // add new signal
        history[history.size() - 1] = input.get_char();

        // check if we just read a marker
        if (i >= (history.size() - 1) && is_marker(history)) {
            std::cout << i + 1 << '\n';
            break;
        }
    }
}