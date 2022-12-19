#include <iostream>
#include <algorithm>
#include <set>
#include <memory>
#include <cstring>
#include <thread>
#include "InputManager.h"

struct Position {
    int x, y;

    bool operator<(const Position &other) const {
        return x == other.x ? y < other.y : x < other.x;
    }
};

template<int width>
struct Grid {
    const static int grid_width = width;

    std::set<Position> state;
    int y_max = 0;

    bool get(Position position) {
        if (position.x < 0 || position.x >= width || position.y <= 0) return true;
        auto iterator = state.find(position);
        if (iterator == state.end()) return false;
        return true;
    }

    bool set(Position position) {
        state.insert(position);
        y_max = std::max(y_max, position.y);
    }
};

struct Shape {
    std::vector<std::vector<int>> shape;

    int width() {
        return shape[0].size();
    }

    int height() {
        return shape.size();
    }

    template<typename consumer>
    void iterate_over_positions(consumer c) {
        for (int y = 0; y < height(); ++y) {
            std::vector<int> &line = shape[y];
            for (int x = 0; x < width(); ++x) {
                if (line[x] != 0) c(x, height() - y - 1);
            }
        }
    }

    bool is_part_of_shape(int x, int y) {
        y = height() - y - 1;
        if (y < 0 || y >= height() || x < 0 || x >= width()) return false;
        return shape[y][x] != 0;
    }
};

using CurrentGrid = Grid<7>;

// no animation
void print_grid(int step, CurrentGrid &grid) {
    std::cout << "== State after " << step << " rocks ==" << std::endl;
    for (int j = 0; j >= -20; --j) {
        for (int u = 0; u < CurrentGrid::grid_width; ++u) {
            std::cout << (grid.get({u, j + grid.y_max}) ? '#' : '.');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// animation
void print_grid(int step, CurrentGrid &grid, Shape &shape, int x, int y) {
    std::stringstream result;
    result << "\033c";
    result << "== State after " << step << " rocks ==" << std::endl;
    for (int j = 6; j >= -20; --j) {
        for (int u = 0; u < CurrentGrid::grid_width; ++u) {
            if (shape.is_part_of_shape(u - x, j + grid.y_max - y)) {
                result << "\033[33mO\033[0m";
            } else {
                result << (grid.get({u, j + grid.y_max}) ? '#' : '.');
            }
        }
        result << std::endl;
    }
    result << std::endl;
    std::cout << result.str();
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Format: " << argv[0] << " <INPUT_FILE> <print|animate>" << std::endl;
        std::cerr << "Animate might NOT work on Windows systems, only tested on Linux!" << std::endl;
        return 1;
    }
    bool animate = std::strcmp(argv[2], "animate") == 0;

    InputManager input(argc, argv);

    std::array<Shape, 5> shapes;
    // #### shape
    shapes[0] = {{{1, 1, 1, 1}}};
    // .#.
    // ###
    // .#. shape
    shapes[1] = {{{0, 1, 0},
                  {1, 1, 1},
                  {0, 1, 0}}};
    // ..#
    // ..#
    // ###
    shapes[2] = {{{0, 0, 1},
                  {0, 0, 1},
                  {1, 1, 1}}};
    // #
    // #
    // #
    // # shape
    shapes[3] = {{{1}, {1}, {1}, {1}}};
    // ##
    // ##
    shapes[4] = {{{1, 1}, {1, 1}}};

    std::vector<bool> jet_pattern;
    input.next_line();
    while (!input.empty()) jet_pattern.push_back(input.get_char() == '>');

    int num_rocks = 2022;
    CurrentGrid grid;

    int t = 0;

    for (int i = 0; i < num_rocks; ++i) {
        Shape shape = shapes[i % shapes.size()];
        int y = grid.y_max + 4;
        int x = 2;
        while (true) {
            bool direction = jet_pattern[t % jet_pattern.size()];
            ++t;
            if (direction) {
                // check if we can move right
                bool can_move = true;
                shape.iterate_over_positions([&grid, &can_move, x, y](int dx, int dy) {
                    if (grid.get({x + dx + 1, y + dy})) can_move = false;
                });
                if (can_move) ++x;
            } else {
                // check if we can move left
                bool can_move = true;
                shape.iterate_over_positions([&grid, &can_move, x, y](int dx, int dy) {
                    if (grid.get({x + dx - 1, y + dy})) can_move = false;
                });
                if (can_move) --x;
            }

            // check if we can move down further
            bool can_move = true;
            shape.iterate_over_positions([&grid, &can_move, x, y](int dx, int dy) {
                if (grid.get({x + dx, y + dy - 1})) can_move = false;
            });
            if (can_move) {
                --y;
                if (animate) {
                    print_grid(i + 1, grid, shape, x, y);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            } else if (!can_move) {
                shape.iterate_over_positions([&grid, x, y](int dx, int dy) {
                    grid.set({x + dx, y + dy});
                });
                break;
            }
        }

        if (!animate && i % 500 == 0) {
            print_grid(i + 1, grid);
        }
    }
    print_grid(num_rocks, grid);

    std::cout << "Hight of tower: " << grid.y_max << std::endl;
}