cmake_minimum_required(VERSION 3.19)
project(BTL_HeDieuHanh)

set(CMAKE_CXX_STANDARD 14)

include_directories(caltrain_project)

add_executable(BTL_HeDieuHanh
        caltrain_project/Caltrain-runner.c
        caltrain_project/Caltrain.c
        caltrain_project/Caltrain.h
        caltrain_project/CMakeLists.txt
        caltrain_project/Makefile
        caltrain_project/projectRequirement.txt
        caltrain_project/reaction-runner.c
        caltrain_project/reaction.c)
