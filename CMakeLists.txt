cmake_minimum_required(VERSION 3.17)
project(wheel)

set(CMAKE_CXX_STANDARD 14)

add_executable(wheel
        keyboard.cpp tracker.cpp)
