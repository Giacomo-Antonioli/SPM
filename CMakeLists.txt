cmake_minimum_required(VERSION 3.21)
project(SPM)

set(CMAKE_CXX_STANDARD 14)

include_directories(g.antonioli3/Project)

add_executable(SPM
        g.antonioli3/Project/Graph.cpp
        g.antonioli3/Project/Graph.h
        g.antonioli3/Project/main.cpp
        g.antonioli3/Project/Node.cpp
        g.antonioli3/Project/Node.h)
