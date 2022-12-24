#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <climits>
#include "InputManager.h"

struct Position {
    int x, y;

    bool operator<(const Position &other) const {
        return x == other.x ? y < other.y : x < other.x;
    }

    bool operator==(Position other) const {
        return x == other.x && y == other.y;
    }
};

struct PathState : public Position {
    int direction;
};

struct Grid {
    std::map<Position, int> cells;
    std::vector<int> min_x, max_x;
    std::vector<int> min_y, max_y;
    std::vector<PathState> path;
    int global_max_x = 0, global_max_y = 0;

    // 0: empty/can't move to, 1: can move to, 2: wall
    int get_state(Position position) {
        auto iterator = cells.find(position);
        if (iterator == cells.end()) return 0;
        return iterator->second;
    }

    void insert(Position position, int state) {
        cells[position] = state;

        if (max_x.size() <= position.y) {
            max_x.push_back(INT_MIN);
            min_x.push_back(INT_MAX);
        }
        if (max_y.size() <= position.x) {
            max_y.push_back(INT_MIN);
            min_y.push_back(INT_MAX);
        }

        if (state != 0) {
            max_x[position.y] = std::max(max_x[position.y], position.x);
            min_x[position.y] = std::min(min_x[position.y], position.x);
            max_y[position.x] = std::max(max_y[position.x], position.y);
            min_y[position.x] = std::min(min_y[position.x], position.y);
        }

        global_max_x = std::max(global_max_x, position.x);
        global_max_y = std::max(global_max_y, position.y);
    }

    std::pair<Position, int> next_position(Position source, int direction) {
        switch (direction % 4) {
            case 0:
                if (max_x[source.y] <= source.x)
                    source.x = min_x[source.y];
                else
                    ++source.x;
                break;
            case 1:
                if (max_y[source.x] <= source.y)
                    source.y = min_y[source.x];
                else
                    ++source.y;
                break;
            case 2:
                if (min_x[source.y] >= source.x)
                    source.x = max_x[source.y];
                else
                    --source.x;
                break;
            case 3:
                if (min_y[source.x] >= source.y)
                    source.y = max_y[source.x];
                else
                    --source.y;
                break;
        }
        return {source, get_state(source)};
    }

    Position move(Position source, int direction, int steps) {
        for (int i = 0; i < steps; ++i) {
            auto [next, state] = next_position(source, direction);
            // hit wall, we are done
            if (state == 2) return source;
            source = next;
            path.push_back({source, direction});
        }
        return source;
    }
};

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    Grid grid;
    for (int y = 0; input.next_line() && !input.empty(); ++y) {
        for (int x = 0; !input.empty(); ++x) {
            switch (input.get_char()) {
                case ' ':
                    grid.insert({x, y}, 0);
                    break;
                case '.':
                    grid.insert({x, y}, 1);
                    break;
                case '#':
                    grid.insert({x, y}, 2);
                    break;
            }
        }
    }

    input.next_line();

    int rotation = 0;
    Position position = {grid.min_x[0], 0};

    int number = 0;
    while (!input.empty()) {
        char current = input.get_char();
        if (current >= '0' && current <= '9') {
            number *= 10;
            number += current - '0';
        } else {
            std::cout << "MOVE " << number << std::endl;
            position = grid.move(position, rotation, number);
            std::cout << "ROTATE " << current << std::endl;
            if (current == 'R') {
                ++rotation;
                if (rotation >= 4) rotation -= 4;
            } else {
                --rotation;
                if (rotation < 0) rotation += 4;
            }
            number = 0;
        }
    }
    if (number != 0) {
        std::cout << "MOVE " << number << std::endl;
        position = grid.move(position, rotation, number);
    }

    for (int y = 0; y <= grid.global_max_y; ++y) {
        for (int x = 0; x <= grid.global_max_x; ++x) {
            auto iterator = std::find(grid.path.begin(), grid.path.end(), PathState{x, y, 0});
            //auto iterator = grid.path.end();
            if (iterator != grid.path.end()) {
                std::cout << "\033[33m";
                switch (iterator->direction % 4) {
                    case 0:
                        std::cout << ">";
                        break;
                    case 1:
                        std::cout << "v";
                        break;
                    case 2:
                        std::cout << "<";
                        break;
                    case 3:
                        std::cout << "^";
                        break;
                }
                std::cout << "\033[0m";
            } else {
                int state = grid.get_state({x, y});
                if (state == 0) std::cout << ' ';
                else if (state == 1) std::cout << '.';
                else std::cout << '#';
            }
        }
        std::cout << std::endl;
    }

    std::cout << "Final position: (" << position.x + 1 << ", " << position.y + 1 << ")" << std::endl;
    std::cout << "Direction: " << rotation << std::endl;
    std::cout << "Score: " << (1000 * (position.y + 1)) + (4 * (position.x + 1)) + (rotation % 4) << std::endl;
}
