cmake_minimum_required(VERSION 3.17)
project(hybridapi)

set(CMAKE_CXX_STANDARD 14)

add_compile_options( -I../src )

add_executable(hybridapi 
    src-stdcpp/main.cpp 

    src/labsland/simulations/watertanksimulation.cpp
)
