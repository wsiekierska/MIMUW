cmake_minimum_required(VERSION 3.23)
project(Kolejki0)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Kolejki0
        kol.cpp
        kol.h
        main.cpp)
