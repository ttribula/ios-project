cmake_minimum_required(VERSION 3.10)
project(IOS C)
set(PROG proj2)
set(FILES_WITHOUT_MAIN components.c components.h)
set(FILES_MAIN proj2.c ${FILES_WITHOUT_MAIN})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -std=c99 -pedantic -lm")
add_executable(${PROG} ${FILES_MAIN})