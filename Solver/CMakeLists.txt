cmake_minimum_required(VERSION 3.0)

project(Solver)

#Solver_Lib
set(Solver_Sources A_Star_Lib.cpp BFS_Lib.cpp)
set(Solver_Headers A_Star_Lib.h Common.h BFS_Lib.h )
# 
add_library(Solver_Lib STATIC ${Solver_Sources} ${Solver_Headers})