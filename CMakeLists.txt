cmake_minimum_required(VERSION 3.23)
project(advent_of_code_2022)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/utility)

add_library(utility utility/InputManager.cpp)

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