cmake_minimum_required(VERSION 3.27)
project(Assignment3)

set(CMAKE_CXX_STANDARD 11)

add_executable(Assignment3 main.cpp
        Queue.cpp
        Stack.cpp
        RoutePlanner.cpp
        Map.cpp)
