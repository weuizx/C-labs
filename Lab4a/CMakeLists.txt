cmake_minimum_required(VERSION 3.20)
project(lab4a C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab4a
        functions.c
        functions.h
        main.c)
