cmake_minimum_required(VERSION 3.0)

project(RoTile_Game)

#packages
find_package(FLTK REQUIRED MODULE)
include(${FLTK_USE_FILE})

if(UNIX)
	find_package (Threads)
endif(UNIX)
#packages
include(${FLTK_USE_FILE})
set(CMAKE_CXX_STACK_SIZE "33554432")

set(Solver_Dir ${CMAKE_CURRENT_SOURCE_DIR}/Solver)


add_subdirectory(Solver)
add_subdirectory(src)