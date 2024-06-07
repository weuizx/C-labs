cmake_minimum_required(VERSION 3.20)
project(lab5 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab5
        functions.c
        functions.h
        main.c)
