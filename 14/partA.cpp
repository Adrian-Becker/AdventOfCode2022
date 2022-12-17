#include <iostream>
#include <algorithm>
#include <thread>
#include <cstring>
#include "InputManager.h"

struct Grid {
    Grid(int width, int height) : width(width), height(height), data(width * height, 0) {}

    // return state of grid at (x,y)
    // 0: air, 1: stone, 2: sand
    char operator()(int x, int y) const {
        if (x < 0 || y < 0 || x >= width || y >= height) return 0;
        return data[x * height + y];
    }

    void set_data(int x, int y, char value) {
        data[x * height + y] = value;
    }

    int width;
    int height;
    std::vector<char> data;
};

int min_x = INT32_MAX;
int max_x = INT32_MIN;
// set to 0, since that's where sand starts dropping
int min_y = 0;
int max_y = INT32_MIN;

// based on the puzzle input a 600x500 grid is created
Grid grid(600, 500);

void print_grid(bool animate) {
    std::stringstream result;
    if (animate) result << "\033c";
    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            int state = grid(x, y);
            if (state == 0) result << '.';
            else if (state == 1) result << '#';
            else result << "\033[33mo\033[0m";
        }
        result << std::endl;
    }

    std::cout << result.str();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Format: " << argv[0] << " <INPUT_FILE> [print|animate]" << std::endl;
        std::cerr << "Animate might NOT work on Windows systems, only tested on Linux!" << std::endl;
        return 1;
    }
    bool animate = std::strcmp(argv[2], "animate") == 0;

    InputManager input(argc, argv);

    // insert rocks
    while (input.next_line()) {
        int x = input.num(",");
        int y = input.num(" -> ");

        while (!input.empty()) {
            int x_target = input.num(",");
            int y_target = input.num(" -> ");

            int dx = x_target > x ? 1 : -1;
            int dy = y_target > y ? 1 : -1;

            while (x != x_target || y != y_target) {
                min_x = std::min(x, min_x);
                min_y = std::min(y, min_y);
                max_x = std::max(x, max_x);
                max_y = std::max(y, max_y);

                grid.set_data(x, y, 1);

                if (x != x_target) x += dx;
                if (y != y_target) y += dy;
            }
        }

        min_x = std::min(x, min_x);
        min_y = std::min(y, min_y);
        max_x = std::max(x, max_x);
        max_y = std::max(y, max_y);

        grid.set_data(x, y, 1);
    }

    bool added = true;
    int num_added = 0;
    // add sand pieces until no more sand can be added
    while (added) {
        int x = 500;
        int y = 0;
        while (true) {
            if (grid(x, y + 1) == 0) {
                ++y;
            } else if (grid(x - 1, y + 1) == 0) {
                --x;
                ++y;
            } else if (grid(x + 1, y + 1) == 0) {
                ++x;
                ++y;
            } else {
                // can't move anymore, so we can stop
                grid.set_data(x, y, 2);
                ++num_added;
                break;
            }

            // sand will fall out of grid, so we stop
            if (y > max_y) {
                added = false;
                break;
            }
        }
        if (animate) {
            print_grid(true);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
    print_grid(animate);
    std::cout << std::endl << "Number of added sand pieces: " << num_added << std::endl;
}