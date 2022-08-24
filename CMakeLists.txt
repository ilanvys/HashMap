cmake_minimum_required(VERSION 3.16)
project(ex6_ilan_vys)

set(CMAKE_CXX_STANDARD 14)

add_executable(ex6_ilan_vys main.cpp HashMap.hpp Dictionary.hpp)
add_executable(tests test_ex6.cpp HashMap.hpp Dictionary.hpp)
