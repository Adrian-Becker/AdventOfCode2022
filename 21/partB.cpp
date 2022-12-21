#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include "InputManager.h"

enum class Operation {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE
};

long reverse_operation(Operation operation, long result, long number, bool first_number) {
    if (first_number) {
        switch (operation) {
            case Operation::PLUS:
                return result - number;
            case Operation::MINUS:
                return result + number;
            case Operation::MULTIPLY:
                return result / number;
            case Operation::DIVIDE:
                return result * number;
        }
        return -1;
    } else {
        switch (operation) {
            case Operation::PLUS:
                return result - number;
            case Operation::MINUS:
                return number - result;
            case Operation::MULTIPLY:
                return result / number;
            case Operation::DIVIDE:
                return number / result;
        }
        return -1;
    }
}

long perform_operation(Operation operation, long number0, long number1) {
    switch (operation) {
        case Operation::PLUS:
            return number0 + number1;
        case Operation::MINUS:
            return number0 - number1;
        case Operation::MULTIPLY:
            return number0 * number1;
        case Operation::DIVIDE:
            return number0 / number1;
    }
    return -1;
}

struct State {
    std::map<std::string, long> numbers;
    std::optional<long> humn;
    std::array<std::string, 2> root_numbers;
    bool switch_mode = false;

    std::optional<long> get(const std::string &string) {
        auto iterator = numbers.find(string);
        if (iterator == numbers.end()) return {};
        return iterator->second;
    }

    void insert(const std::string &string, long value) {
        if (string == "humn") {
            humn = value;
        }

        if (string == root_numbers[0] || string == root_numbers[1]) {
            numbers[root_numbers[0]] = value;
            numbers[root_numbers[1]] = value;
            switch_mode = true;
        }

        numbers[string] = value;
    }
};

struct Monkey {
    std::string name;
    std::string partA;
    std::string partB;
    Operation operation;

    bool attempt(State &state) const {
        std::optional<long> number1 = state.get(partA);
        if (!number1.has_value()) return false;
        std::optional<long> number2 = state.get(partB);
        if (!number2.has_value()) return false;

        state.insert(name, perform_operation(operation, *number1, *number2));

        return true;
    }

    bool reverse_attempt(State &state) const {
        std::optional<long> result = state.get(name);
        if (!result.has_value()) return false;

        std::optional<long> number1 = state.get(partA);
        std::optional<long> number2 = state.get(partB);
        if (!number1.has_value() && !number2.has_value()) return false;

        if (number1.has_value()) {
            state.insert(partB, reverse_operation(operation, *result, *number1, false));
        } else {
            state.insert(partA, reverse_operation(operation, *result, *number2, true));
        }

        return true;
    }
};

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    // read input
    std::list<Monkey> monkeys;
    State state;
    while (input.next_line()) {
        std::string name = input.substring(4);
        input.skip(2);
        if (name == "root") {
            std::string partA = input.substring(4);
            input.skip(3);
            std::string partB = input.substring(4);
            state.root_numbers = {partA, partB};
            continue;
        }
        if (name == "humn") continue;

        if (input.contains(" ")) {
            // is operation
            std::string partA = input.substring(4);
            input.skip(1);
            Operation operation;
            switch (input.get_char()) {
                case '+':
                    operation = Operation::PLUS;
                    break;
                case '-':
                    operation = Operation::MINUS;
                    break;
                case '*':
                    operation = Operation::MULTIPLY;
                    break;
                case '/':
                    operation = Operation::DIVIDE;
                    break;
            }
            input.skip(1);
            std::string partB = input.substring(4);
            monkeys.push_back({
                                      name,
                                      partA,
                                      partB,
                                      operation
                              });
        } else {
            // is number
            state.insert(name, input.line_as_num());
        }
    }

    std::cout << monkeys.size() << " monkey left undetermined after initialization!" << std::endl;

    int i;
    for (i = 1; !monkeys.empty() && !state.switch_mode; ++i) {
        // try to find monkey number based on inputs
        monkeys.remove_if([&state](Monkey &monkey) { return monkey.attempt(state); });
        std::cout << "After " << i << " rounds, " << monkeys.size() << " remain." << std::endl;
    }
    std::cout << "One root number was found." << std::endl;
    for (; !monkeys.empty() && !state.humn.has_value(); ++i) {
        // try to find one input based on the monkey number and one input
        monkeys.remove_if([&state](Monkey &monkey) { return monkey.reverse_attempt(state); });
        std::cout << "After " << i << " rounds, " << monkeys.size() << " remain." << std::endl;
    }

    std::cout << "Human number found, the value is " << *state.humn << "." << std::endl;
}
