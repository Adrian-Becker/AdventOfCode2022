#include <iostream>
#include <array>
#include <algorithm>
#include "InputManager.h"

struct UpdateRule {
    bool power = false;
    bool product = true;
    int value = 1;

    int operator()(int old) const {
        if (power) {
            return old * old;
        } else if (product) {
            return old * value;
        } else {
            return old + value;
        }
    }
};

struct Monkey {
    int id;
    std::vector<int> items{};
    UpdateRule update;
    int divider;
    int result_true;
    int result_false;
    int inspected = 0;

    // method expects Monkey to not throw items at themselves
    void step(std::vector<Monkey> &monkeys) {
        for (int item: items) {
            item = update(item);
            item /= 3;
            if (item % divider == 0) {
                monkeys[result_true].items.push_back(item);
            } else {
                monkeys[result_false].items.push_back(item);
            }
            ++inspected;
        }
        items.clear();
    }


};

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

    for (int i = 0; i < 20; ++i) {
        for (auto &monkey: monkeys) {
            monkey.step(monkeys);
        }
    }

    // sort the monkeys by the number of items they have inspected
    // they are sorted by their id, if they have inspected the same number of items
    // not the most efficient way to do this but quite easy
    std::sort(monkeys.begin(), monkeys.end(), [](const Monkey& a, const Monkey &b) {
        if (a.inspected == b.inspected) return a.id > b.id;
        return a.inspected > b.inspected;
    });

    std::cout << monkeys[0].inspected * monkeys[1].inspected << std::endl;
}