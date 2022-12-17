#include <iostream>
#include <algorithm>
#include <thread>
#include <cstring>
#include <set>
#include "InputManager.h"

struct Position {
    long x, y;

    bool operator<(const Position &other) const {
        return (x == other.x ? y < other.y : x < other.x);
    }
};

using Interval = Position;

long distance(const Position &a, const Position &b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

struct Sensor {
    Position position_sensor, position_beacon;
    long length = distance(position_sensor, position_beacon);
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


    auto start = std::chrono::steady_clock::now();
    int y_max = 4000000;
    for (long y = 0; y <= y_max; ++y) {
        // build intervals of impossible ranges
        std::vector<Interval> intervals;
        for (Sensor &sensor: sensors) {
            long dist = std::abs(sensor.position_sensor.y - y);
            if (dist <= sensor.length) {
                long extend = sensor.length - dist;
                intervals.push_back({sensor.position_sensor.x - extend, sensor.position_sensor.x + extend});
            }
        }

        std::sort(intervals.begin(), intervals.end(), [](Interval a, Interval b) {
            return a.x == b.x ? a.y < b.y : a.x < b.x;
        });

        long x = 0;
        for (Interval &interval: intervals) {
            if (interval.x > x) {
                std::cout << "Point found: (" << x << "," << y << ")" << std::endl;
                std::cout << "Frequency: " << ((long) x * 4000000 + (long) y) << std::endl;
                goto end_loop;
            }
            x = std::max(x, interval.y + 1);
        }
        if (x <= y_max && x > intervals[intervals.size() - 1].y) {
            std::cout << "Point found: (" << x << "," << y << ")" << std::endl;
            std::cout << "Frequency: " << ((long) x * 4000000 + (long) y) << std::endl;
            goto end_loop;
        }

        if (y % 500000 == 499999) {
            auto end = std::chrono::steady_clock::now();
            std::cout << "Finished y<=" << y << std::endl;
            long seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            double remaining = (double) seconds * (double)(y_max - y) / (double)(y + 1);
            std::cout << "Approximately " << remaining << "ms remaining!" << std::endl;
        }
    }

    end_loop:
    return 0;
}