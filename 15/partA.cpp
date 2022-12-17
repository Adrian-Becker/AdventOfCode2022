#include <iostream>
#include <algorithm>
#include <thread>
#include <cstring>
#include <set>
#include "InputManager.h"

struct Position {
    int x, y;

    bool operator<(const Position &other) const {
        return (x == other.x ? y < other.y : x < other.x);
    }
};

int distance(const Position &a, const Position &b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

struct Sensor {
    Position position_sensor, position_beacon;

    int length() {
        return distance(position_sensor, position_beacon);
    }
};

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    std::vector<Sensor> sensors;
    while (input.next_line()) {
        // Skip "Sensor at x="
        input.skip(12);
        int sensor_x = input.num(", y=");
        int sensor_y = input.num(": closest beacon is at x=");
        int beacon_x = input.num(", y=");
        int beacon_y = input.line_as_num();

        sensors.push_back({{sensor_x, sensor_y},
                           {beacon_x, beacon_y}});
    }

    int y_level = 2000000;
    std::set<Position> save_points;
    for (Sensor &sensor: sensors) {
        Position check{sensor.position_sensor.x, y_level};
        int length = sensor.length();
        if (distance(check, sensor.position_sensor) <= length) {
            save_points.insert(check);
            for (int i = 1; true; ++i) {
                if (distance({check.x + i, y_level}, sensor.position_sensor) <= length) {
                    save_points.insert({check.x + i, y_level});
                    save_points.insert({check.x - i, y_level});
                } else break;
            }
        }
    }

    for (Sensor &sensor : sensors) {
        save_points.erase(sensor.position_beacon);
        save_points.erase(sensor.position_sensor);
    }

    std::cout << "Save points: " << save_points.size() << std::endl;
}