#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <climits>
#include "InputManager.h"

struct Position {
    int x, y;

    bool operator<(const Position &other) const {
        return x == other.x ? y < other.y : x < other.x;
    }

    bool operator==(const Position &other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position &other) const {
        return !(operator==(other));
    }

    Position operator+(const Position &other) const {
        return {
                x + other.x,
                y + other.y
        };
    }

    Position &operator+=(const Position &other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};

struct Elv {
    Position position;
    Position target;
    bool wants_to_move;
};

struct Grid {
    std::vector<Elv> elves;
    std::map<Position, int> targets;
    int x_min = 0, x_max = 0;
    int y_min = 0, y_max = 0;

    bool find_elv(Position position) {
        return std::any_of(elves.begin(), elves.end(), [position](Elv &elv) { return elv.position == position; });
    }

    void step() {
        x_min = INT_MAX;
        x_max = INT_MIN;

        y_min = INT_MAX;
        y_max = INT_MIN;

        targets.clear();
        pre_round();
        move();

        for (Elv &elv: elves) {
            x_min = std::min(x_min, elv.position.x);
            y_min = std::min(y_min, elv.position.y);

            x_max = std::max(x_max, elv.position.x);
            y_max = std::max(y_max, elv.position.y);
        }
    }

private:
    std::vector<Position> neighbours{
            {-1, -1},
            {0,  -1},
            {1,  -1},
            {1,  0},
            {1,  1},
            {0,  1},
            {-1, 1},
            {-1, 0}
    };

    void move() {
        for (Elv &elv: elves) {
            // doesn't want to move
            if (!elv.wants_to_move) continue;
            // two or more elves want to move here, can't move
            if (targets[elv.target] > 1) continue;
            elv.position = elv.target;
        }
    }

    int rotation = 0;

    void pre_round() {
        for (Elv &elv: elves) {
            int num_free = 0;
            std::array<bool, 8> free{};
            for (int i = 0; i < neighbours.size(); ++i) {
                if (!find_elv(elv.position + neighbours[i])) {
                    ++num_free;
                    free[i] = true;
                } else {
                    free[i] = false;
                }
            }

            // all free, don't move
            if (num_free == 8) {
                elv.wants_to_move = false;
                continue;
            }

            bool found_target = false;

            std::array<Position, 4> directions{Position{0, 0},
                                               Position{0, 0},
                                               Position{0, 0},
                                               Position{0, 0}};

            if (free[0] && free[1] && free[2]) {
                // move north
                found_target = true;
                directions[(0 + rotation) % 4] = {0, -1};
            }
            if (free[4] && free[5] && free[6]) {
                // move south
                found_target = true;
                directions[(1 + rotation) % 4] = {0, 1};
            }
            if (free[6] && free[7] && free[0]) {
                // move west
                found_target = true;
                directions[(2 + rotation) % 4] = {-1, 0};
            }
            if (free[2] && free[3] && free[4]) {
                // move east
                found_target = true;
                directions[(3 + rotation) % 4] = {1, 0};
            }

            if (found_target) {
                Position &target = elv.target;
                elv.target = elv.position;
                for (Position &move: directions) {
                    if (move != Position{0, 0}) {
                        target += move;
                        break;
                    }
                }
                targets[target] += 1;
                elv.wants_to_move = true;
            } else {
                elv.wants_to_move = false;
            }
        }

        --rotation;
        if (rotation < 0) rotation += 4;
    }
};

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    Grid grid;
    for (int y = 0; input.next_line(); ++y) {
        for (int x = 0; !input.empty(); ++x) {
            char current = input.get_char();
            if (current == '#')
                grid.elves.push_back({
                                             {x, y},
                                             {0, 0},
                                             false
                                     });
        }
    }

    // do 10 steps
    for (int i = 0; i < 10; ++i) {
        grid.step();
        // print grid & calculate the number of free fields
        std::cout << "== State after Round " << (i + 1) << " ==" << std::endl;
        for (int y = grid.y_min; y <= grid.y_max; ++y) {
            for (int x = grid.x_min; x <= grid.x_max; ++x) {
                if (grid.find_elv({x, y})) {
                    std::cout << "#";
                } else {
                    std::cout << '.';
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // print grid & calculate the number of free fields
    int free_fields = 0;
    for (int y = grid.y_min; y <= grid.y_max; ++y) {
        for (int x = grid.x_min; x <= grid.x_max; ++x) {
            if (!grid.find_elv({x, y})) {
                ++free_fields;
            }
        }
    }
    std::cout << "Number of free fields: " << free_fields << std::endl;
}
