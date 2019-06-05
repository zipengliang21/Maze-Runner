cmake_minimum_required(VERSION 3.13)
project(Maze)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Maze
        maze.cpp
        maze.h
        dsets.cpp
        dsets.h
        main.cpp utility/HSLAPixel.cpp utility/HSLAPixel.h utility/PNG.cpp utility/PNG.h utility/lodepng.cpp utility/lodepng.h utility/RGB_HSL.h)
