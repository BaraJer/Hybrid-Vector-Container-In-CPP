cmake_minimum_required(VERSION 3.20)
project(ex7_gfat)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex7_gfat
        vl_vector.h
        test_runner.h
        test_runner.cpp
        tests.cpp)
