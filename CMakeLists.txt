cmake_minimum_required(VERSION 3.0)

project(RoTile_Game)

if(NOT WIN32)
	if(NOT CMAKE_BUILD_TYPE)
	  set(CMAKE_BUILD_TYPE Release)
	endif()

	set(CMAKE_CXX_FLAGS "-Wall")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()
#packages
set(FLTK_SKIP_OPENGL TRUE)
set(FLTK_SKIP_FORMS TRUE)
set(FLTK_SKIP_FLUID TRUE)
find_package(FLTK REQUIRED)
#if(NOT MSYS)
	include_directories(${FLTK_INCLUDE_DIRS})
#endif(NOT MSYS)

if(UNIX)
	find_package (Threads)
endif(UNIX)
#packages
set(CMAKE_CXX_STACK_SIZE "33554432")

set(Solver_Dir ${CMAKE_CURRENT_SOURCE_DIR}/Solver)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


add_subdirectory(Solver)
add_subdirectory(src)
