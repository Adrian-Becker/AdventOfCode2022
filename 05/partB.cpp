#include <iostream>
#include <list>
#include "InputManager.h"

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);
    input.next_line();

    std::vector<std::list<char>> current_state;
    for (int i = 0; i < (input.line().length() + 1) / 3; ++i) current_state.emplace_back(std::list<char>{});

    // read the initial state
    while (input.line()[1] != '1') {
        input.skip(1);
        for (int i = 0; !input.empty(); ++i) {
            char current_container = input.get_char();
            if (current_container != ' ') {
                current_state[i].push_front(current_container);
            }
            input.skip(3);
        }

        input.next_line();
    }

    input.next_line();

    while (input.next_line()) {
        // skip "move "
        input.skip(5);
        int num = input.num(" from ");
        int from = input.num(" to ") - 1;
        int to = input.line_as_num() - 1;

        // ensure that current_state has the right size
        int max = std::max(from, to) + 1;
        while (current_state.size() < max) {
            current_state.emplace_back(std::list<char>{});
        }

        // move the elements
        std::list<char> elements_to_move;
        for (int i = 0; i < num; ++i) {
            elements_to_move.push_front(current_state[from].back());
            current_state[from].pop_back();
        }
        for (char container : elements_to_move) {
            current_state[to].push_back(container);
        }
    }

    for (const auto &list : current_state) {
        std::cout << list.back();
    }
    std::cout << std::endl;
}