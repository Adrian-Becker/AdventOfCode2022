#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <climits>
#include <cmath>
#include "InputManager.h"

struct Position {
    int x, y;

    bool operator<(const Position &other) const {
        return x == other.x ? y < other.y : x < other.x;
    }

    bool operator==(const Position &other) const {
        return x == other.x && y == other.y;
    }
};

struct PathState : public Position {
    int direction;
};

struct Transition {
    Position source;
    int direction_source;
    Position target;
    int direction_target;

    std::tuple<Position, int> translate(Position position, int face_size) {
        position.x -= source.x * face_size;
        position.y -= source.y * face_size;

        // calculate position as if the position is at top edge
        if (direction_source == 0) {
            position.x = position.x;
            position.y = 0;
        } else if (direction_source == 1) {
            position.x = position.y;
            position.y = 0;
        } else if (direction_source == 2) {
            position.x = face_size - 1 - position.x;
            position.y = 0;
        } else if (direction_source == 3) {
            position.x = face_size - 1 - position.y;
            position.y = 0;
        }

        // translate to the correct edge
        if (direction_target == 0) {
            position.x = face_size - 1 - position.x;
            position.y = 0;
        } else if (direction_target == 1) {
            position.y = face_size - 1 - position.x;
            position.x = face_size - 1;
        } else if (direction_target == 2) {
            position.y = face_size - 1;
        } else if (direction_target == 3) {
            position.y = position.x;
            position.x = 0;
        }

        position.x += face_size * target.x;
        position.y += face_size * target.y;

        return {position, direction_target};
    }
};

struct Grid {
    std::map<Position, int> cells;
    std::vector<PathState> path;
    int global_max_x = 0, global_max_y = 0;
    int face_size;
    std::vector<Transition> transitions;


    // 0: empty/can't move to, 1: can move to, 2: wall
    int get_state(Position position) {
        auto iterator = cells.find(position);
        if (iterator == cells.end()) return 0;
        return iterator->second;
    }

    void insert(Position position, int state) {
        cells[position] = state;

        global_max_x = std::max(global_max_x, position.x);
        global_max_y = std::max(global_max_y, position.y);
    }

    std::tuple<Position, int, int> next_position(Position source, int direction) {
        Position source_face{source.x / face_size, source.y / face_size};
        Position target = source;
        switch (direction % 4) {
            case 0:
                ++target.x;
                break;
            case 1:
                ++target.y;
                break;
            case 2:
                --target.x;
                break;
            case 3:
                --target.y;
                break;
        }
        int state = get_state(target);
        if (state == 0) {
            ++direction;
            if (direction >= 4) direction -= 4;

            bool found = false;
            for (auto &transition: transitions) {
                if (transition.source == source_face && transition.direction_source == direction) {
                    std::cout << "FOUND\n";
                    auto [target_new, direction_new] = transition.translate(source, face_size);
                    target = target_new;
                    direction = direction_new;
                    state = get_state(target);
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "NOT FOUND ";
                std::cout << source_face.x << " " << source_face.y << " " << direction << std::endl;
            }

            ++direction;
            if (direction >= 4) direction -= 4;
        }

        return {target, state, direction};
    }

    Position move(Position source, int &direction, int steps) {
        for (int i = 0; i < steps; ++i) {
            auto [next, state, direction_new] = next_position(source, direction);
            // hit wall, we are done
            if (state == 2) return source;
            source = next;
            path.push_back({source, direction_new});
            direction = direction_new;
        }
        return source;
    }
};

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    Grid grid;
    int number_tiles = 0;
    for (int y = 0; input.next_line() && !input.empty(); ++y) {
        for (int x = 0; !input.empty(); ++x) {
            switch (input.get_char()) {
                case ' ':
                    grid.insert({x, y}, 0);
                    break;
                case '.':
                    grid.insert({x, y}, 1);
                    ++number_tiles;
                    break;
                case '#':
                    grid.insert({x, y}, 2);
                    ++number_tiles;
                    break;
            }
        }
    }
    int face_size = (int) std::sqrt(number_tiles / 6);
    grid.face_size = face_size;

    if (face_size == 4) {
        // example case
        grid.transitions.push_back({
                                           {2, 0}, 0,
                                           {0, 1}, 0
                                   });
        grid.transitions.push_back({
                                           {2, 0}, 1,
                                           {3, 2}, 1
                                   });
        grid.transitions.push_back({
                                           {2, 0}, 3,
                                           {1, 1}, 0
                                   });
        grid.transitions.push_back({
                                           {0, 1}, 0,
                                           {2, 0}, 0
                                   });
        grid.transitions.push_back({
                                           {0, 1}, 2,
                                           {2, 2}, 2
                                   });
        grid.transitions.push_back({
                                           {0, 1}, 3,
                                           {3, 2}, 2
                                   });
        grid.transitions.push_back({
                                           {1, 1}, 0,
                                           {2, 0}, 3
                                   });
        grid.transitions.push_back({
                                           {1, 1}, 2,
                                           {2, 2}, 3
                                   });
        grid.transitions.push_back({
                                           {2, 1}, 1,
                                           {3, 2}, 0
                                   });
        grid.transitions.push_back({
                                           {2, 2}, 2,
                                           {0, 1}, 2
                                   });
        grid.transitions.push_back({
                                           {2, 2}, 3,
                                           {1, 1}, 2
                                   });
        grid.transitions.push_back({
                                           {3, 2}, 0,
                                           {2, 1}, 1
                                   });
        grid.transitions.push_back({
                                           {3, 2}, 1,
                                           {2, 0}, 1
                                   });
        grid.transitions.push_back({
                                           {3, 2}, 2,
                                           {0, 1}, 3
                                   });
    } else {
        // real input case
        grid.transitions.push_back({
                                           {2, 0}, 0,
                                           {0, 3}, 2
                                   });
        grid.transitions.push_back({
                                           {0, 3}, 2,
                                           {2, 0}, 0
                                   });
        grid.transitions.push_back({
                                           {2, 0}, 1,
                                           {1, 2}, 1
                                   });
        grid.transitions.push_back({
                                           {1, 2}, 1,
                                           {2, 0}, 1
                                   });
        grid.transitions.push_back({
                                           {2, 0}, 2,
                                           {1, 1}, 1
                                   });
        grid.transitions.push_back({
                                           {1, 1}, 1,
                                           {2, 0}, 2
                                   });
        grid.transitions.push_back({
                                           {1, 0}, 0,
                                           {0, 3}, 3
                                   });
        grid.transitions.push_back({
                                           {0, 3}, 3,
                                           {1, 0}, 0
                                   });
        grid.transitions.push_back({
                                           {1, 0}, 3,
                                           {0, 2}, 3
                                   });
        grid.transitions.push_back({
                                           {0, 2}, 3,
                                           {1, 0}, 3
                                   });
        grid.transitions.push_back({
                                           {1, 1}, 3,
                                           {0, 2}, 0
                                   });
        grid.transitions.push_back({
                                           {0, 2}, 0,
                                           {1, 1}, 3
                                   });
        grid.transitions.push_back({
                                           {1, 2}, 2,
                                           {0, 3}, 1
                                   });
        grid.transitions.push_back({
                                           {0, 3}, 1,
                                           {1, 2}, 2
                                   });
    }

    input.next_line();

    int rotation = 0;
    Position position = {0, 0};
    for (position.x = 0; grid.get_state(position) == 0; ++position.x);

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
