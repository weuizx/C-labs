cmake_minimum_required(VERSION 3.20)
project(lab4b C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab4b
        functions.c
        functions.h
        main.c)
