#include <iostream>
#include "InputManager.h"

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    input.next_line();
    // create matrix of trees and initializes with the correct size
    std::vector<std::vector<int>> trees(input.line().size(), std::vector<int>{});

    // add data to it
    do {
        for (int i = 0; !input.empty(); ++i) {
            trees[i].push_back(input.get_char() - '0');
        }
    } while (input.next_line());

    // matrix of visible trees
    int y_max = trees[0].size();
    std::vector<bool> empty_data(y_max, false);
    std::vector<std::vector<bool>> visible(trees.size(), empty_data);

    // the following steps assume that the grid of trees is a square
    for (int i = 0; i < trees.size(); ++i) {
        int max_size_left = -1;
        int max_size_right = -1;
        int max_size_top = -1;
        int max_size_bottom = -1;

        for (int j = 0; j < trees.size(); ++j) {
            if (trees[j][i] > max_size_left) {
                visible[j][i] = true;
                max_size_left = trees[j][i];
            }
            if (trees[trees.size() - 1 - j][i] > max_size_right) {
                visible[trees.size() - 1 - j][i] = true;
                max_size_right = trees[trees.size() - 1 - j][i];
            }
            if (trees[i][j] > max_size_top) {
                visible[i][j] = true;
                max_size_top = trees[i][j];
            }
            if (trees[i][trees.size() - 1 - j] > max_size_bottom) {
                visible[i][trees.size() - 1 - j] = true;
                max_size_bottom = trees[i][trees.size() - 1 - j];
            }
        }
    }

    // sum up number of visible trees
    int count_visible = 0;
    for (int i = 0; i < trees.size(); ++i) {
        for (int j = 0; j < trees.size(); ++j) {
            if (visible[i][j]) ++count_visible;
        }
    }

    std::cout << count_visible << std::endl;
}