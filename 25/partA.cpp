#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <climits>
#include <cstring>
#include <thread>
#include <chrono>
#include <iomanip>
#include <complex>
#include "InputManager.h"

long long snafu_to_decimal(const std::string &input) {
    long long result = 0;
    for (int i = 0; i < input.size(); ++i) {
        result *= 5;
        char current = input[i];
        if (current == '-') {
            result -= 1;
        } else if (current == '=') {
            result -= 2;
        } else {
            result += current - '0';
        }
    }

    std::cout << std::setw(20) << input << '\t' << std::setw(20) << result;

    // print the way to calculate this result, only optical
    std::cout << "\033[32m = ";
    for (int i = 0; i < input.size(); ++i) {
        char current = input[i];
        if (current == '-') {
            std::cout << "-1";
        } else if (current == '=') {
            std::cout << "-2";
        } else {
            std::cout << ' ' << current;
        }
        std::cout << " * 5^" << (input.size() - i - 1);
        if (i + 1 < input.size()) std::cout << " + ";
    }
    std::cout << "\033[0m" << std::endl;

    return result;
}

long long power(long long base, int exponent) {
    long long result = 1;
    while (exponent != 0) {
        if (exponent % 2 == 0) {
            base *= base;
            exponent /= 2;
        } else {
            --exponent;
            result *= base;
        }
    }
    return result;
}

std::string decimal_to_snafu(long long number) {
    // we want to convert the number to a base 5 number, with = = 0, - = 1, 0 = 2, 1 = 3, 2 = 4
    // to do so, we add 2*5^i for all 0 <= i < length of number and than convert it

    long long converted_number = number;

    // find correct length
    int length = 0;
    while (converted_number > power(5, length) - 1) {
        converted_number += 2 * power(5, length);
        ++length;
    }
    std::cout << "The length of the Decimal number \033[32m" << number << "\033[0m in SNAFU is " << length << std::endl;

    // convert to base 5 with alternative symbols
    std::string result(length, 'x');
    for (int i = 0; i < length; ++i) {
        long long remainder = (converted_number % 5) - 2;
        converted_number /= 5;

        if (remainder == -2) {
            result[length - 1 - i] = '=';
        } else if (remainder == -1) {
            result[length - 1 - i] = '-';
        } else {
            result[length - 1 - i] = '0' + remainder;
        }
    }
    std::cout << "The Decimal number \033[32m" << number << "\033[0m equals the SNAFU number \033[32m" << result << "\033[0m" << std::endl;

    return result;
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    long long sum = 0;
    std::cout << std::setw(20) << "SNAFU" << '\t' << std::setw(20) << "Decimal" << std::endl;
    while (input.next_line()) {
        sum += snafu_to_decimal(input.line());
    }

    std::cout << std::endl << "Sum of all numbers: " << sum << std::endl << std::endl;

    decimal_to_snafu(sum);
}
