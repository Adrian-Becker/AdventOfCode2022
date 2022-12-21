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
    std::optional<long> root;

    std::optional<long> get(const std::string &string) {
        auto iterator = numbers.find(string);
        if (iterator == numbers.end()) return {};
        return iterator->second;
    }

    void insert(const std::string &string, long value) {
        if (string == "root") {
            root = value;
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
};

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    // read input
    std::list<Monkey> monkeys;
    State state;
    while (input.next_line()) {
        std::string name = input.substring(4);
        input.skip(2);
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

    for (int i = 1; !monkeys.empty() && !state.root.has_value(); ++i) {
        monkeys.remove_if([&state](Monkey &monkey) { return monkey.attempt(state); });
        std::cout << "After " << i << " rounds, " << monkeys.size() << " remain." << std::endl;
    }

    std::cout << "Root found, the value is " << *state.root << "." << std::endl;
}
