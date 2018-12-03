cmake_minimum_required(VERSION 3.0)

project(RoTile_Game)

find_package(FLTK REQUIRED MODULE)
include(${FLTK_USE_FILE})
set(CMAKE_CXX_STACK_SIZE "33554432")

if(UNIX)
	find_package (Threads)
endif(UNIX)


set(Sources RoTile_Game.cpp BoxesPreferences.cpp)
set(Headers RoTile_Game.h BoxesPreferences.h)

#A_Star_Lib
set(A_Star_Lib_Sources A_Star_Lib.cpp)
set(A_Star_Lib_Headers A_Star_Lib.h Common.h)
# 
add_library(A_Star_Lib STATIC ${A_Star_Lib_Sources} ${A_Star_Lib_Headers})
#Bfs
set(BFS_Lib_Sources BFS_Lib.cpp)
set(BFS_Lib_Headers BFS_Lib.h Common.h)
#
add_library(BFS_Lib STATIC ${BFS_Lib_Sources} ${BFS_Lib_Headers})

if(MSVC_IDE)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RoTile_Game)
endif(MSVC_IDE)
add_executable(RoTile_Game ${Sources} ${Headers})
add_dependencies(RoTile_Game BFS_Lib A_Star_Lib)
target_link_libraries(RoTile_Game fltk fltk_images A_Star_Lib BFS_Lib) 

if(UNIX)
	target_link_libraries (RoTile_Game ${CMAKE_THREAD_LIBS_INIT})
endif(UNIX)