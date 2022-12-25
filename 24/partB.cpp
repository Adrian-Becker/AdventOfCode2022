#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <climits>
#include <cstring>
#include <thread>
#include <chrono>
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

struct Blizzard {
    Position position;
    Position direction;
};

struct Grid {
    int width;
    int height;

    std::vector<bool> state;
    std::vector<Blizzard> blizzards;

    void set_size(int width, int height) {
        this->width = width;
        this->height = height;
        state.clear();
        state.resize(width * height, false);
    }

    // update blizzard positions and mark fields that can't be entered
    void step() {
        std::fill(state.begin(), state.end(), false);

        for (Blizzard &blizzard: blizzards) {
            blizzard.position += blizzard.direction;

            if (blizzard.position.y >= height) blizzard.position.y = 0;
            else if (blizzard.position.y < 0) blizzard.position.y = height - 1;
            else if (blizzard.position.x >= width) blizzard.position.x = 0;
            else if (blizzard.position.x < 0) blizzard.position.x = width - 1;

            state[blizzard.position.x * height + blizzard.position.y] = true;
        }
    }

    bool get_state(Position position) {
        if (position.x < 0 || position.x >= width || position.y < 0 || position.y >= height) return true;
        return state[position.x * height + position.y];
    }

    void display(std::string &title, int step, std::set<Position> &current, std::set<Position> &history) {
        std::stringstream result;
        result << "\033c";

        result << "== " << title << " ==" << std::endl << std::endl;

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (current.find({j, i}) != current.end()) {
                    result << "\033[32mO\033[0m";
                } else if (history.find({j, i}) != history.end()) {
                    result << "\033[33m" << (get_state({j, i}) ? '#' : '/') << "\033[0m";
                } else {
                    result << (get_state({j, i}) ? "#" : "/");
                }
            }
            result << std::endl;
        }
        result << std::endl;
        result << "Step: " << step << std::endl;
        result << "Positions, covered currently: " << current.size() << " of " << (height * width + 1) << std::endl;
        result << "Positions, covered in the past: " << history.size() << " of " << (height * width + 1) << std::endl;

        std::cout << result.str();
    }
};

int find_path(Grid &grid, bool animate, Position start_pos, Position end_condition, std::string title) {
    std::set<Position> current{start_pos};
    // only populated if animate is active
    std::set<Position> history{};

    int steps = 1;
    while (true) {
        std::set<Position> next{};

        grid.step();

        for (const Position &position: current) {
            if (position == start_pos) next.insert(position);
            if (position == end_condition) {
                return steps;
            }

            // stay still
            Position check = position;
            if (!grid.get_state(check)) {
                next.insert(check);
            }

            // position below
            check += Position{0, 1};
            if (!grid.get_state(check)) {
                next.insert(check);
            }

            // position above
            check += Position{0, -2};
            if (!grid.get_state(check)) {
                next.insert(check);
            }

            // position left
            check += Position{-1, 1};
            if (!grid.get_state(check)) {
                next.insert(check);
            }

            // position right
            check += Position{2, 0};
            if (!grid.get_state(check)) {
                next.insert(check);
            }
        }

        std::swap(current, next);
        if (animate) {
            grid.display(title, steps, current, history);
            history.insert(current.begin(), current.end());
            std::this_thread::sleep_for(std::chrono::milliseconds(70));
        }

        ++steps;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Format: " << argv[0] << " <INPUT_FILE> <print|animate>" << std::endl;
        std::cerr << "Animate might NOT work on Windows systems, only tested on Linux!" << std::endl;
        return 1;
    }
    bool animate = std::strcmp(argv[2], "animate") == 0;

    InputManager input(argc, argv);

    input.next_line();
    int width = (int) input.size() - 2;
    int height = -1;

    Grid grid;
    for (int y = 0; input.next_line(); ++y) {
        input.skip(1);
        for (int x = 0; !input.empty(); ++x) {
            char current = input.get_char();
            // end reached
            if (current == '#') {
                if (x == 0) {
                    height = y;
                    goto end_reading_file;
                }
            } else if (current == '>') {
                grid.blizzards.push_back({
                                                 {x, y},
                                                 {1, 0}
                                         });
            } else if (current == '<') {
                grid.blizzards.push_back({
                                                 {x,  y},
                                                 {-1, 0}
                                         });
            } else if (current == 'v') {
                grid.blizzards.push_back({
                                                 {x, y},
                                                 {0, 1}
                                         });
            } else if (current == '^') {
                grid.blizzards.push_back({
                                                 {x, y},
                                                 {0, -1}
                                         });
            }
        }
    }
    end_reading_file:
    grid.set_size(width, height);

    int steps =
            find_path(grid, animate, {0, -1}, {grid.width - 1, grid.height - 1}, "start -> end") +
            find_path(grid, animate, {grid.width - 1, grid.height}, {0, 0}, "end -> start") +
            find_path(grid, animate, {0, -1}, {grid.width - 1, grid.height - 1}, "start -> end");

    if (animate) std::cout << std::endl;
    std::cout << "Size of field: " << width << "x" << height << std::endl;
    std::cout << "Required number of steps: " << steps << std::endl;
}
