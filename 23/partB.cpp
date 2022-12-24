#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <set>
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
    std::set<Position> positions;
    std::map<Position, int> targets;
    int x_min = 0, x_max = 0;
    int y_min = 0, y_max = 0;

    bool find_elv(Position position) {
        return positions.find(position) != positions.end();
    }

    bool step() {
        x_min = INT_MAX;
        x_max = INT_MIN;

        y_min = INT_MAX;
        y_max = INT_MIN;

        targets.clear();
        pre_round();
        bool result = move();

        for (Elv &elv: elves) {
            x_min = std::min(x_min, elv.position.x);
            y_min = std::min(y_min, elv.position.y);

            x_max = std::max(x_max, elv.position.x);
            y_max = std::max(y_max, elv.position.y);
        }

        return result;
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

    bool move() {
        bool moved = false;
        positions.clear();
        for (Elv &elv: elves) {
            // doesn't want to move or
            // two or more elves want to move here, can't move
            if (!elv.wants_to_move || targets[elv.target] > 1) {
                positions.insert(elv.position);
                continue;
            }
            elv.position = elv.target;
            positions.insert(elv.position);
            moved = true;
        }
        return moved;
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
            if (current == '#') {
                grid.elves.push_back({
                                             {x, y},
                                             {0, 0},
                                             false
                                     });
                grid.positions.insert({x, y});
            }
        }
    }

    int round;
    for (round = 1; true; ++round) {
        if (grid.step()) {
            std::cout << "Elves moved in round " << round << std::endl;
        } else {
            break;
        }
    }

    std::cout << "Elves had to move for " << round << " rounds" << std::endl;
}
