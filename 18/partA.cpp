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
};

struct Grid {
    std::set<Position> state;

    bool get(Position position) {
        auto iterator = state.find(position);
        if (iterator == state.end()) return false;
        return true;
    }

    void set(Position position) {
        state.insert(position);
    }
};

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    Grid grid;
    // add all cubes to grid
    while (input.next_line()) {
        grid.set({input.num(","), input.num(","), input.line_as_num()});
    }

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

    std::cout << "Total surface of droplet: " << surface << std::endl;
}
