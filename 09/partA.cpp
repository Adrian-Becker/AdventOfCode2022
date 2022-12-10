#include <iostream>
#include <set>
#include "InputManager.h"

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    // set so we don't count visited grid positions twice
    std::set<std::pair<int, int>> visited;
    visited.insert({0, 0});

    std::pair<int, int> head{0, 0};
    std::pair<int, int> tail{0, 0};

    while (input.next_line()) {
        char direction = input.get_char();
        input.skip(1);
        int count = input.line_as_num();

        for (int i = 0; i < count; ++i) {
            // change head position based on direction
            if (direction == 'R') ++head.first;
            else if (direction == 'L') --head.first;
            else if (direction == 'U') ++head.second;
            else --head.second;

            int dx = head.first - tail.first;
            int dy = head.second - tail.second;
            // update position if distance between head and tail is at least 2
            if (dy == 2 || dy == -2) {
                tail.first += dx;
                tail.second += dy / 2;
            } else if (dx == 2 || dx == -2) {
                tail.first += dx / 2;
                tail.second += dy;
            }

            visited.insert(tail);
        }
    }

    std::cout << visited.size() << std::endl;
}