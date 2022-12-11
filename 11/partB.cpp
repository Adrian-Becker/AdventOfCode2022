#include <iostream>
#include <array>
#include <algorithm>
#include "InputManager.h"

struct UpdateRule {
    bool power = false;
    bool product = true;
    int value = 1;

    long operator()(long old) const {
        if (power) {
            return old * old;
        } else if (product) {
            return old * value;
        } else {
            return old + value;
        }
    }
};

long lcm = -1;

struct Monkey {
    int id;
    std::vector<long> items{};
    UpdateRule update;
    int divider;
    int result_true;
    int result_false;
    int inspected = 0;

    // method expects Monkey to not throw items at themselves
    void step(std::vector<Monkey> &monkeys) {
        for (long item: items) {
            item = update(item);
            if (item % divider == 0) {
                // we can use modulo lcm since products and sums and squaring is equal under modulo
                // when checking for divisibility
                monkeys[result_true].items.push_back(item % lcm);
            } else {
                // we can use modulo lcm since products and sums and squaring is equal under modulo
                // when checking for divisibility
                monkeys[result_false].items.push_back(item % lcm);
            }
            ++inspected;
        }
        items.clear();
    }


};

long calculate_lcm(long n1, long n2) {
    long max = std::max(n1, n2);

    while (true) {
        if (max % n1 == 0 && max % n2 == 0) {
            return max;
        } else {
            ++max;
        }
    }
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    std::vector<Monkey> monkeys;
    for (int i = 0; input.next_line(); ++i) {
        input.next_line();

        Monkey monkey{};
        monkey.id = i;

        // skip "  Starting items: "
        input.skip(18);
        // parse starting items
        while (!input.empty()) monkey.items.push_back(input.num(", "));

        input.next_line();
        // skip "  Operation: new = old "
        input.skip(23);
        monkey.update.product = input.get_char() == '*';
        input.skip(1);
        if (input.line() == "old") monkey.update.power = true;
        else monkey.update.value = input.line_as_num();

        input.next_line();
        // skip "  Test: divisible by "
        input.skip(21);
        monkey.divider = input.line_as_num();

        if (lcm == -1) lcm = monkey.divider;
        else lcm = calculate_lcm(lcm, monkey.divider);

        input.next_line();
        // skip "    If true: throw to monkey "
        input.skip(29);
        monkey.result_true = input.line_as_num();

        input.next_line();
        // skip "    If false: throw to monkey "
        input.skip(30);
        monkey.result_false = input.line_as_num();

        input.next_line();

        monkeys.push_back(monkey);
    }

    int iteration = 0;
    std::array<int, 12> iterations{1, 20, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
    for (int iteration_count: iterations) {
        for (; iteration < iteration_count; ++iteration) {
            for (auto &monkey: monkeys) {
                monkey.step(monkeys);
            }
        }

        std::cout << "== After round " << iteration_count << " ==" << std::endl;
        for (int i = 0; i < monkeys.size(); ++i) {
            std::cout << "Monkey " << i << " inspected items " << monkeys[i].inspected << " times." << std::endl;
        }
        std::cout << std::endl;
    }

    // sort the monkeys by the number of items they have inspected
    // they are sorted by their id, if they have inspected the same number of items
    // not the most efficient way to do this but quite easy
    std::sort(monkeys.begin(), monkeys.end(), [](const Monkey& a, const Monkey &b) {
        if (a.inspected == b.inspected) return a.id > b.id;
        return a.inspected > b.inspected;
    });

    std::cout << "Monkey Business: " << (long long) monkeys[0].inspected * (long long) monkeys[1].inspected << std::endl;
}