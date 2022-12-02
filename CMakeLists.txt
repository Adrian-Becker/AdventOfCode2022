cmake_minimum_required(VERSION 3.23)
project(advent_of_code_2022)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/utility)

add_library(utility utility/InputManager.cpp)

add_executable(day01a 01/partA.cpp)
target_link_libraries(day01a utility)

add_executable(day01b 01/partB.cpp)
target_link_libraries(day01b utility)

add_executable(day02a 02/partA.cpp)
target_link_libraries(day02a utility)

add_executable(day02b 02/partB.cpp)
target_link_libraries(day02b utility)