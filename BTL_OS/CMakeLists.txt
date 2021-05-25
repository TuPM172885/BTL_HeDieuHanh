cmake_minimum_required(VERSION 3.19)
project(System_Operation_Project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS -pthread)

#include_directories(.)
#include_directories(cmake-build-debug)

add_executable(System_Operation_Project Caltrain-runner.c)
