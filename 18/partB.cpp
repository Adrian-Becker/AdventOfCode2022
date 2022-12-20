#include <algorithm>
#include <set>
#include <iostream>
#include "InputManager.h"

struct Position {
    int x, y, z;

    bool operator<(const Position &other) const {
        return x == other.x ? (
                y == other.y ? z < other.z : y < other.y
        ) : x < other.x;
    }

    Position add(int dx, int dy, int dz) {
        return {x + dx, y + dy, z + dz};
    }
};

struct Box {
    int x_min = 0, y_min = 0, z_min = 0;
    int x_max = 0, y_max = 0, z_max = 0;

    bool is_inside(int x, int y, int z) {
        return x >= x_min && y >= y_min && z >= z_min &&
               x <= x_max && y <= y_max && z <= z_max;
    }

    bool is_inside(Position position) {
        return is_inside(position.x, position.y, position.z);
    }
};

struct Grid {
    std::set<Position> state;
    Box bounding_box;

    bool get(Position position) {
        auto iterator = state.find(position);
        if (iterator == state.end()) return false;
        return true;
    }

    void set(Position position) {
        state.insert(position);

        bounding_box.x_min = std::min(bounding_box.x_min, position.x);
        bounding_box.y_min = std::min(bounding_box.y_min, position.y);
        bounding_box.z_min = std::min(bounding_box.z_min, position.z);

        bounding_box.x_max = std::max(bounding_box.x_max, position.x);
        bounding_box.y_max = std::max(bounding_box.y_max, position.y);
        bounding_box.z_max = std::max(bounding_box.z_max, position.z);
    }
};

int get_total_surface(Grid &grid) {
    int surface = 0;
    // check all cubes
    for (auto &position: grid.state) {
        if (!grid.get({position.x + 1, position.y, position.z})) ++surface;
        if (!grid.get({position.x - 1, position.y, position.z})) ++surface;
        if (!grid.get({position.x, position.y + 1, position.z})) ++surface;
        if (!grid.get({position.x, position.y - 1, position.z})) ++surface;
        if (!grid.get({position.x, position.y, position.z + 1})) ++surface;
        if (!grid.get({position.x, position.y, position.z - 1})) ++surface;
    }

    return surface;
}

int get_total_inner_surface(Grid &grid, Box &bounding_box) {
    int surface = 0;
    // check all cubes
    for (auto &position: grid.state) {
        if (!bounding_box.is_inside(position)) continue;
        if (!grid.get({position.x + 1, position.y, position.z})) ++surface;
        if (!grid.get({position.x - 1, position.y, position.z})) ++surface;
        if (!grid.get({position.x, position.y + 1, position.z})) ++surface;
        if (!grid.get({position.x, position.y - 1, position.z})) ++surface;
        if (!grid.get({position.x, position.y, position.z + 1})) ++surface;
        if (!grid.get({position.x, position.y, position.z - 1})) ++surface;
    }

    return surface;
}

void flood(Grid &grid, Box &bounding_box, Position position) {
    grid.set(position);

    if (bounding_box.is_inside(position.add(1, 0, 0)) && !grid.get(position.add(1, 0, 0)))
        flood(grid, bounding_box, position.add(1, 0, 0));
    if (bounding_box.is_inside(position.add(-1, 0, 0)) && !grid.get(position.add(-1, 0, 0)))
        flood(grid, bounding_box, position.add(-1, 0, 0));
    if (bounding_box.is_inside(position.add(0, 1, 0)) && !grid.get(position.add(0, 1, 0)))
        flood(grid, bounding_box, position.add(0, 1, 0));
    if (bounding_box.is_inside(position.add(0, -1, 0)) && !grid.get(position.add(0, -1, 0)))
        flood(grid, bounding_box, position.add(0, -1, 0));
    if (bounding_box.is_inside(position.add(0, 0, 1)) && !grid.get(position.add(0, 0, 1)))
        flood(grid, bounding_box, position.add(0, 0, 1));
    if (bounding_box.is_inside(position.add(0, 0, -1)) && !grid.get(position.add(0, 0, -1)))
        flood(grid, bounding_box, position.add(0, 0, -1));
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    Grid grid;
    // add all cubes to grid
    while (input.next_line()) {
        grid.set({input.num(","), input.num(","), input.line_as_num()});
    }

    int total_surface = get_total_surface(grid);

    // build bounding_box box so that every outer point can be reached
    // by any other and save old bounding box
    Box old_bounding_box = grid.bounding_box;
    Box bounding_box = grid.bounding_box;
    --bounding_box.x_min;
    --bounding_box.y_min;
    --bounding_box.z_min;
    ++bounding_box.x_max;
    ++bounding_box.y_max;
    ++bounding_box.z_max;
    flood(grid, bounding_box, {bounding_box.x_min, bounding_box.y_min, bounding_box.z_min});

    int inner_surface = get_total_inner_surface(grid, old_bounding_box);

    int outer_surface = total_surface - inner_surface;

    std::cout << "Total surface of droplet: " << outer_surface << std::endl;
}
