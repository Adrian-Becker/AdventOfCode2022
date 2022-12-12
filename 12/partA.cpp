#include <iostream>
#include <array>
#include <algorithm>
#include <list>
#include <cstring>
#include <thread>
#include "InputManager.h"

struct Position {
    int x;
    int y;

    bool operator==(Position other) const {
        return x == other.x && y == other.y;
    }
};

std::vector<Position> trace_path(Position start, Position end, const std::vector<std::vector<Position>> &parent) {
    std::vector<Position> path;
    for (Position current = end; current.x >= 0; current = parent[current.x][current.y]) {
        path.push_back(current);
    }
    return path;
}

void check_position(Position new_position, const std::vector<std::vector<int>> &height_map,
                    std::vector<std::vector<Position>> &parent, std::vector<std::vector<int>> &cost,
                    std::list<Position> &todo,
                    int current_height, int current_cost, Position current_position) {
    // check that points are in bound
    if (new_position.x < 0 || new_position.x >= height_map.size() || new_position.y < 0 ||
        new_position.y >= height_map[0].size())
        return;
    // allow a maximum of one step upwards
    if (height_map[new_position.x][new_position.y] - current_height <= 1) {
        int cost_new = current_cost + 1;
        if (cost_new < cost[new_position.x][new_position.y]) {
            // parent not set <=> hasn't been added to the todo_list yet
            if (parent[new_position.x][new_position.y].x == -1) todo.push_back(new_position);
            parent[new_position.x][new_position.y] = current_position;
            cost[new_position.x][new_position.y] = cost_new;
            return;
        }
    }
}

// performs a star algorithm
std::vector<Position> a_star(Position start, Position end, const std::vector<std::vector<int>> &height_map) {
    std::list<Position> todo;
    todo.push_back(start);
    std::vector<std::vector<Position>> parent(height_map.size(),
                                              std::vector<Position>(height_map[0].size(), Position{-1, -1}));
    parent[start.x][start.x] = {-2, -2};
    std::vector<std::vector<int>> cost(height_map.size(), std::vector<int>(height_map[0].size(), INT32_MAX));
    cost[start.x][start.y] = 0;

    while (!todo.empty()) {
        auto current_position = todo.begin();
        Position current = *current_position;
        // we guess the cost using the current cost + the absolute distance in x and y, since that's the minimum
        // number of steps required to reach the end (if such a path exists)
        int cost_guess = cost[current.x][current.y] + std::abs(current.x - end.x) + std::abs(current.y - end.y);

        // find the node with the current lowest cost guess
        for (auto iterator = todo.begin(); iterator != todo.end(); ++iterator) {
            Position node = *iterator;
            int node_cost_guess = cost[node.x][node.y] + std::abs(node.x - end.x) + std::abs(node.y - end.y);
            if (node_cost_guess < cost_guess) {
                current_position = iterator;
                current = node;
                cost_guess = node_cost_guess;
            }
        }
        todo.erase(current_position);

        // return path once end has been reached
        if (current == end) {
            return trace_path(start, end, parent);
        }

        // check neighbours
        int current_height = height_map[current.x][current.y];
        int current_cost = cost[current.x][current.y];

        check_position(Position{current.x + 1, current.y}, height_map, parent, cost, todo,
                       current_height, current_cost, current);
        check_position(Position{current.x - 1, current.y}, height_map, parent, cost, todo,
                       current_height, current_cost, current);
        check_position(Position{current.x, current.y + 1}, height_map, parent, cost, todo,
                       current_height, current_cost, current);
        check_position(Position{current.x, current.y - 1}, height_map, parent, cost, todo,
                       current_height, current_cost, current);
    }
    // no path found
    return {};
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Format: " << argv[0] << " <INPUT_FILE> [print|animate]" << std::endl;
        std::cerr << "Animate might NOT work on Windows systems, only tested on Linux!" << std::endl;
        return 1;
    }
    bool animate = std::strcmp(argv[2], "animate") == 0;

    InputManager input(argc, argv);

    Position start{-1, -1};
    Position end{-1, -1};

    std::vector<std::vector<int>> height_map;
    for (int i = 0; input.next_line(); ++i) {
        std::vector<int> current_line;
        for (int j = 0; !input.empty(); ++j) {
            char current = input.get_char();
            if (current == 'S') {
                start.x = i;
                start.y = j;
                current_line.emplace_back(0);
            } else if (current == 'E') {
                end.x = i;
                end.y = j;
                current_line.emplace_back(25);
            } else {
                current_line.emplace_back(current - 'a');
            }
        }
        height_map.push_back(current_line);
    }

    std::vector<Position> path = a_star(start, end, height_map);

    std::string color_scale = "@%&M*admQUcxt(}]-+<il;,^' ";
    for (int k = animate ? 0 : ((int) path.size() - 1); k < path.size(); ++k) {
        // we save the result in a string buffer first,
        // so we can easily write it to cout without triggering a flush
        // after every line
        std::stringstream result;
        if (animate) result << "\033c";
        for (int i = 0; i < height_map.size(); ++i) {
            const std::vector<int> &current = height_map[i];
            for (int j = 0; j < current.size(); ++j) {
                char symbol = color_scale[25 - current[j]];
                if (start.x == i && start.y == j) {
                    // print S in red and bold
                    result << "\033[1m\033[31mS\033[0m";
                } else if (end.x == i && end.y == j) {
                    // print E in green and bold
                    result << "\033[1m\033[32mE\033[0m";
                } else if (std::find(path.begin() + (path.size() - k), path.end(), Position{i, j}) !=
                           path.end()) {
                    // print path symbols in yellow
                    result << "\033[33m" << symbol << "\033[0m";
                } else {
                    result << symbol;
                }
            }
            result << std::endl;
        }
        std::cout << result.str();
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    std::cout << std::endl;

    std::cout << "Steps required: " << path.size() - 1 << std::endl;
}