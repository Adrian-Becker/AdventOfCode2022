#include <iostream>
#include <algorithm>
#include <memory>
#include <utility>
#include "InputManager.h"

struct Packet {
    bool is_value;
    int value;
    std::vector<std::shared_ptr<Packet>> packets;
    // these two values are only used for console outputs
    std::string text_representation;
    int depth;

    explicit Packet(int value, int depth) : value(value), packets({}), text_representation(std::to_string(value)),
                                            depth(depth), is_value(true) {}

    explicit Packet(std::string list, int depth) : value(-1), packets({}), text_representation(list), depth(depth),
                                                   is_value(false) {
        insert_data(std::move(list));
    }

    void insert_data(std::string list) {
        LineInput input(std::move(list));
        if (input.line()[0] == '[') {
            input.skip(1);
            input.skip(-1);
            while (!input.empty()) {
                std::string next = input.substring(",");
                if (next[0] == '[') {
                    // insert a list of values
                    while (std::count(next.begin(), next.end(), '[') != std::count(next.begin(), next.end(), ']'))
                        next += "," + input.substring(",");
                }
                packets.push_back(std::make_shared<Packet>(next, depth + 1));
            }
        } else {
            // input is just a value
            is_value = true;
            value = input.line_as_num();
        }
    }

    // prepare a line with indentation to print
    auto &line() {
        std::cout << std::string(depth, '\t');
        return std::cout;
    }

    // -1: left bigger
    // 0: same size
    // 1: right bigger
    int compare(Packet &other) {
        line() << "- Compare " << text_representation << " vs " << other.text_representation << std::endl;

        // both packets are just values
        // only values need to be compared
        if (is_value && other.is_value) {
            return value == other.value ? 0 : (value < other.value ? 1 : -1);
        }

        // fix mixed types (only one is value and the other one a list)
        if (is_value && !other.is_value) {
            packets.push_back(std::make_shared<Packet>(value, depth + 1));
            is_value = false;
            line() << "\t- Mixed types; convert left to [" << value << "] and retry comparison" << std::endl;
        } else if (!is_value && other.is_value) {
            other.packets.push_back(std::make_shared<Packet>(other.value, depth + 1));
            other.is_value = false;
            line() << "\t- Mixed types; convert right to [" << value << "] and retry comparison" << std::endl;
        }

        // check values of list of subpackets
        for (int i = 0; i < std::min(packets.size(), other.packets.size()); ++i) {
            auto &packet1 = *packets[i];
            auto &packet2 = *other.packets[i];
            int result = packet1.compare(packet2);
            if (result != 0) {
                if (result == 1)
                    line() << "\t- Left side is smaller, so inputs are in the right order" << std::endl;
                else line() << "\t- Right side is smaller, so inputs are not in the right order" << std::endl;
                return result;
            }
        }

        // all subpackets are equal, compare based on the number of subpackets
        if (packets.size() > other.packets.size() || (depth == 0 && packets.size() == other.packets.size()))
            line() << "\t- Right side ran out of items, so inputs are not in the right order" << std::endl;
        else line() << "\t- Left side ran out of items, so inputs are in the right order" << std::endl;
        return packets.size() == other.packets.size() ? 0 : (packets.size() < other.packets.size() ? 1 : -1);
    }
};

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    int sum_right_order = 0;
    for (int i = 1; input.next_line(); ++i) {
        Packet packet0(input.line(), 0);
        input.next_line();
        Packet packet1(input.line(), 0);
        input.next_line();

        std::cout << "== Pair " << i << " ==" << std::endl;
        int result = packet0.compare(packet1);
        std::cout << std::endl;

        if (result == 1) {
            sum_right_order += i;
        }
    }

    std::cout << "Result: " << sum_right_order;
}