cmake_minimum_required(VERSION 3.21)
project(SPM)

set(CMAKE_CXX_STANDARD 14)

include_directories(g.antonioli3/Project)

add_executable(SPM
        g.antonioli3/Project/Sequential/Graph.cpp
        g.antonioli3/Project/Sequential/Graph.h
        g.antonioli3/Project/Sequential/main.cpp
        g.antonioli3/Project/Sequential/Node.cpp
        g.antonioli3/Project/Sequential/Node.h)
