#include <iostream>
#include <algorithm>
#include <set>
#include <memory>
#include <cstring>
#include <thread>
#include "InputManager.h"

struct Position {
    long x, y;

    bool operator<(const Position &other) const {
        return x == other.x ? y < other.y : x < other.x;
    }
};

template<int width>
struct Grid {
    const static int grid_width = width;

    std::set<Position> state;
    long y_max = 0;

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

int main(int argc, char *argv[]) {
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

    long num_rocks = 1000000000000;
    CurrentGrid grid;

    std::vector<std::pair<long, int>> states;

    std::cout << "Generating first 30000 iterations to look for cycles in." << std::endl;
    int t = 0;
    // insert 10000 rocks to then check for cycles
    for (long i = 0; i < 30000; ++i) {
        Shape shape = shapes[i % shapes.size()];
        long y = grid.y_max + 4;
        int x = 2;

        if (i % shapes.size() == 0) {
            states.emplace_back(grid.y_max, t % jet_pattern.size());
        }
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
            } else if (!can_move) {
                shape.iterate_over_positions([&grid, x, y](int dx, int dy) {
                    grid.set({x + dx, y + dy});
                });
                break;
            }
        }
    }

    long cycle_start_height = 0;
    long cycle_offset_stones = 0;
    long cycle_length = 0;
    long cycle_increase = 0;
    std::vector<int> modulo_size_increase;

    std::cout << "Searching for a cycle." << std::endl;
    // detect cycles
    for (int len = 1; len < states.size() / 2; ++len) {
        std::pair<long, int> state0 = states[states.size() - 1 - 2 * len];
        std::pair<long, int> state1 = states[states.size() - 1 - 1 * len];
        std::pair<long, int> state2 = states[states.size() - 1];
        long height0 = state0.first;
        long height1 = state1.first;
        long height2 = state2.first;

        // not the same height / position in jet stream
        if (height1 - height0 != height2 - height1 || state0.second != state1.second) continue;

        bool is_cycle = true;
        // check that all positions in the grid are the same
        for (int j = 0; j < height1 - height0; ++j) {
            for (int u = 0; u < CurrentGrid::grid_width; ++u) {
                if (grid.get({u, height0 + j}) != grid.get({u, height1 + j})) {
                    is_cycle = false;
                    break;
                }
            }
        }
        if (is_cycle) {
            cycle_start_height = height0;
            cycle_offset_stones = ((long) states.size() - 1 - 2 * len) * 5;
            cycle_length = 5 * len;
            cycle_increase = height1 - height0;
            for (int i = 0; i < len; ++i) {
                modulo_size_increase.push_back(states[states.size() - 1 - 2 * len + i].first - height0);
            }

            std::cout << "Cycle detected, length " << 5 * len << " starting at " << height0 << "." << std::endl;
            break;
        }
    }

    long rocks_to_add = num_rocks - cycle_offset_stones;
    long cycles = rocks_to_add / cycle_length;
    long remaining = (rocks_to_add % cycle_length) / 5;
    long total_height = cycle_start_height + cycles * cycle_increase + modulo_size_increase[remaining];

    std::cout << "Height of tower: " << total_height << std::endl;
}