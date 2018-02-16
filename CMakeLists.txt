cmake_minimum_required(VERSION 3.9)
project(Uni_Chess_Project C)

set(CMAKE_C_STANDARD 99)

add_executable(Uni_Chess_Project main.c Auxiliary.c Auxiliary.h Game.c Game.h Board.c Board.h)