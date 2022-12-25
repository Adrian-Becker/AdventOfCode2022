cmake_minimum_required(VERSION 3.22)
project(advent_of_code_2022)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/utility)

add_library(utility utility/InputManager.cpp utility/LineInput.cpp)

# link utility to all target
link_libraries(utility)

# day 1
add_executable(day01a 01/partA.cpp)
add_executable(day01b 01/partB.cpp)

# day 2
add_executable(day02a 02/partA.cpp)
add_executable(day02b 02/partB.cpp)

# day 3
add_executable(day03a 03/partA.cpp)
add_executable(day03b 03/partB.cpp)

# day 4
add_executable(day04a 04/partA.cpp)
add_executable(day04b 04/partB.cpp)

# day 5
add_executable(day05a 05/partA.cpp)
add_executable(day05b 05/partB.cpp)

# day 6
add_executable(day06a 06/partA.cpp)
add_executable(day06b 06/partB.cpp)

# day 7
add_executable(day07a 07/partA.cpp)
add_executable(day07b 07/partB.cpp)

# day 8
add_executable(day08a 08/partA.cpp)
add_executable(day08b 08/partB.cpp)

# day 9
add_executable(day09a 09/partA.cpp)
add_executable(day09b 09/partB.cpp)

# day 10
add_executable(day10a 10/partA.cpp)
add_executable(day10b 10/partB.cpp)

# day 11
add_executable(day11a 11/partA.cpp)
add_executable(day11b 11/partB.cpp)

# day 12
add_executable(day12a 12/partA.cpp)
add_executable(day12b 12/partB.cpp)

# day 13
add_executable(day13a 13/partA.cpp)
add_executable(day13b 13/partB.cpp)

# day 14
add_executable(day14a 14/partA.cpp)
add_executable(day14b 14/partB.cpp)

# day 15
add_executable(day15a 15/partA.cpp)
add_executable(day15b 15/partB.cpp)

# day 16
add_executable(day16a 16/partA.cpp)
add_executable(day16b 16/partB.cpp)

# day 17
add_executable(day17a 17/partA.cpp)
add_executable(day17b 17/partB.cpp)

# day 18
add_executable(day18a 18/partA.cpp)
add_executable(day18b 18/partB.cpp)

# day 19
add_executable(day19a 19/partA.cpp)
add_executable(day19b 19/partB.cpp)

# day 20
add_executable(day20a 20/partA.cpp)
add_executable(day20b 20/partB.cpp)

# day 21
add_executable(day21a 21/partA.cpp)
add_executable(day21b 21/partB.cpp)

# day 22
add_executable(day22a 22/partA.cpp)
add_executable(day22b 22/partB.cpp)

# day 23
add_executable(day23a 23/partA.cpp)
add_executable(day23b 23/partB.cpp)

# day 24
add_executable(day24a 24/partA.cpp)
add_executable(day24b 24/partB.cpp)

# day 25
add_executable(day25a 25/partA.cpp)