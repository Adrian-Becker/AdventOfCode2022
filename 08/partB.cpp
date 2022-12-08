#include <iostream>
#include "InputManager.h"

int calculate_scenic_score(int i, int j, const std::vector<std::vector<int>> &trees) {
    int height = trees[i][j];
    int score = 1;
    // looking left
    {
        int current_score = 0;
        for (int di = i - 1; di >= 0; --di) {
            ++current_score;
            if (trees[di][j] >= height) break;
        }
        score *= current_score;
    }
    // looking right
    {
        int current_score = 0;
        for (int di = i + 1; di < trees.size(); ++di) {
            ++current_score;
            if (trees[di][j] >= height) break;
        }
        score *= current_score;
    }
    // looking down
    {
        int current_score = 0;
        for (int dj = j - 1; dj >= 0; --dj) {
            ++current_score;
            if (trees[i][dj] >= height) break;
        }
        score *= current_score;
    }
    // looking up
    {
        int current_score = 0;
        for (int dj = j + 1; dj < trees.size(); ++dj) {
            ++current_score;
            if (trees[i][dj] >= height) break;
        }
        score *= current_score;
    }
    return score;
}

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
    // iterating over all trees is probably not the fastest way to calculate
    // the max scenic score
    int max_score = 0;
    for (int i = 0; i < trees.size(); ++i) {
        for (int j = 0; j < trees.size(); ++j) {
            max_score = std::max(calculate_scenic_score(i, j, trees), max_score);
        }
    }

    std::cout << max_score << std::endl;
}