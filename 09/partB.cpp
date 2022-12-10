#include <iostream>
#include <set>
#include <array>
#include "InputManager.h"

void simulate_knot(std::pair<int, int> &head, std::pair<int, int> &tail) {
    int dx = head.first - tail.first;
    int dy = head.second - tail.second;

    // update position if distance between head and tail is at least 2
    if ((dy == 2 || dy == -2) && (dx == 2 || dx == -2)) {
        // diagonal case
        tail.first += dx / 2;
        tail.second += dy / 2;
    } else if (dy == 2 || dy == -2) {
        tail.first += dx;
        tail.second += dy / 2;
    } else if (dx == 2 || dx == -2) {
        tail.first += dx / 2;
        tail.second += dy;
    }
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    // set so we don't count visited grid positions twice
    std::set<std::pair<int, int>> visited;
    visited.insert({0, 0});

    std::array<std::pair<int, int>, 10> knots;
    for (int i = 0; i < 10; ++i) knots[i] = {0, 0};

    while (input.next_line()) {
        char direction = input.get_char();
        input.skip(1);
        int count = input.line_as_num();

        for (int i = 0; i < count; ++i) {
            // change head position based on direction
            if (direction == 'R') ++knots[0].first;
            else if (direction == 'L') --knots[0].first;
            else if (direction == 'U') ++knots[0].second;
            else --knots[0].second;

            for (int j = 0; j < 9; ++j) {
                simulate_knot(knots[j], knots[j + 1]);
            }

            visited.insert(knots[9]);
        }
    }

    std::cout << visited.size() << std::endl;
}