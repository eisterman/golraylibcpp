cmake_minimum_required(VERSION 3.30)
project(golraylibcpp)

set(CMAKE_CXX_STANDARD 23)

find_package(raylib)

add_executable(golraylibcpp main.cpp
        GameOfLife.hpp)

target_link_libraries(golraylibcpp raylib)