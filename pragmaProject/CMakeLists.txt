cmake_minimum_required(VERSION 3.15)
project(pragmaProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(pragmaProject main.cpp)