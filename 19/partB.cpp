#include <algorithm>
#include <iostream>
#include <map>
#include "InputManager.h"

struct State {
    int ore_robots = 1;
    int clay_robots = 0;
    int obsidian_robots = 0;
    int geode_robots = 0;

    int ore = 0;
    int clay = 0;
    int obsidian = 0;
    int geodes = 0;

    void step(int n) {
        ore += n * ore_robots;
        clay += n * clay_robots;
        obsidian += n * obsidian_robots;
        geodes += n * geode_robots;
    }

    bool operator<(const State &other) const {
        if (ore != other.ore) return ore < other.ore;
        if (clay != other.clay) return clay < other.clay;
        if (obsidian != other.obsidian) return obsidian < other.obsidian;
        if (geodes != other.geodes) return geodes < other.geodes;
        if (ore_robots != other.ore_robots) return ore_robots < other.ore_robots;
        if (clay_robots != other.clay_robots) return clay_robots < other.clay_robots;
        if (obsidian_robots != other.obsidian_robots) return obsidian_robots < other.obsidian_robots;
        return geode_robots < other.geode_robots;
    }
};

struct Cost {
    int ore = 0;
    int clay = 0;
    int obsidian = 0;

    bool can_potentially_buy(State &state) const {
        if (clay != 0 && state.clay_robots == 0) return false;
        if (obsidian != 0 && state.obsidian_robots == 0) return false;
        return true;
    }

    int necessary_steps(State &state) const {
        int needed_ore = ore - state.ore;
        int needed_clay = clay - state.clay;
        int needed_obsidian = obsidian - state.obsidian;

        int n = 0;
        if (needed_ore > 0) n = std::max(n, (needed_ore + state.ore_robots - 1) / state.ore_robots);
        if (needed_clay > 0) n = std::max(n, (needed_clay + state.clay_robots - 1) / state.clay_robots);
        if (needed_obsidian > 0) n = std::max(n, (needed_obsidian + state.obsidian_robots - 1) / state.obsidian_robots);
        return n;
    }

    void buy(State &state) const {
        state.ore -= ore;
        state.clay -= clay;
        state.obsidian -= obsidian;
    }
};

struct Blueprint {
    Cost ore_robot;
    Cost clay_robot;
    Cost obsidian_robot;
    Cost geode_robot;

    int max_ore;
    int max_clay;
    int max_obsidian;

    void calculate_maximums() {
        max_ore = std::max({ore_robot.ore, clay_robot.ore, obsidian_robot.ore, geode_robot.ore});
        max_clay = std::max({ore_robot.clay, clay_robot.clay, obsidian_robot.clay, geode_robot.clay});
        max_obsidian = std::max(
                {ore_robot.obsidian, clay_robot.obsidian, obsidian_robot.obsidian, geode_robot.obsidian});
    }
};

int max_number_geodes(Blueprint &blueprint, int remaining_time, State &state) {
    if (remaining_time <= 0) return state.geodes;

    int max_geodes = 0;
    bool buy = false;
    if (blueprint.geode_robot.can_potentially_buy(state)) {
        int steps = blueprint.geode_robot.necessary_steps(state) + 1;
        if (steps <= remaining_time) {
            State new_state = state;
            new_state.step(steps);
            blueprint.geode_robot.buy(new_state);
            ++new_state.geode_robots;
            max_geodes = std::max(max_geodes, max_number_geodes(blueprint, remaining_time - steps, new_state));
            buy = true;
        }
    }
    if (state.obsidian_robots < blueprint.max_obsidian && blueprint.obsidian_robot.can_potentially_buy(state)) {
        int steps = blueprint.obsidian_robot.necessary_steps(state) + 1;
        if (steps <= remaining_time) {
            State new_state = state;
            new_state.step(steps);
            blueprint.obsidian_robot.buy(new_state);
            ++new_state.obsidian_robots;
            max_geodes = std::max(max_geodes, max_number_geodes(blueprint, remaining_time - steps, new_state));
            buy = true;
        }
    }
    if (state.clay_robots < blueprint.max_clay && blueprint.clay_robot.can_potentially_buy(state)) {
        int steps = blueprint.clay_robot.necessary_steps(state) + 1;
        if (steps <= remaining_time) {
            State new_state = state;
            new_state.step(steps);
            blueprint.clay_robot.buy(new_state);
            ++new_state.clay_robots;
            max_geodes = std::max(max_geodes, max_number_geodes(blueprint, remaining_time - steps, new_state));
            buy = true;
        }
    }
    if (state.ore_robots < blueprint.max_ore && blueprint.ore_robot.can_potentially_buy(state)) {
        int steps = blueprint.ore_robot.necessary_steps(state) + 1;
        if (steps <= remaining_time) {
            State new_state = state;
            new_state.step(steps);
            blueprint.ore_robot.buy(new_state);
            ++new_state.ore_robots;
            max_geodes = std::max(max_geodes, max_number_geodes(blueprint, remaining_time - steps, new_state));
            buy = true;
        }
    }

    if (!buy) {
        State new_state = state;
        new_state.step(remaining_time);
        return new_state.geodes;
    }

    return max_geodes;
}

int max_number_geodes(Blueprint &blueprint, int time) {
    State state;
    return max_number_geodes(blueprint, time, state);
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    long total_score = 1;

    for (int i = 1; i <= 3; ++i) {
        input.next_line();
        Blueprint blueprint;
        input.substring(" robot costs ");
        blueprint.ore_robot.ore = input.num(" ore. Each clay robot costs ");
        blueprint.clay_robot.ore = input.num(" ore. Each obsidian robot costs ");
        blueprint.obsidian_robot.ore = input.num(" ore and ");
        blueprint.obsidian_robot.clay = input.num(" clay. Each geode robot costs ");
        blueprint.geode_robot.ore = input.num(" ore and ");
        blueprint.geode_robot.obsidian = input.num(" obsidian.");
        blueprint.calculate_maximums();

        int score = max_number_geodes(blueprint, 32);

        std::cout << "Max number of mined geodes for blueprint " << i << " is " << score << std::endl;
        total_score *= score;
    }

    std::cout << "Total quality level: " << total_score << std::endl;
}
