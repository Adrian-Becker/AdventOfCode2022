#ifndef AOC2022_INPUTMANAGER_H
#define AOC2022_INPUTMANAGER_H

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "LineInput.h"

class InputManager : public LineInput {
public:
    InputManager(int argc, char *argv[]);

    // checks whether there is another line and moves to the next line
    bool next_line();
private:
    std::ifstream input;
};


#endif //AOC2022_INPUTMANAGER_H
