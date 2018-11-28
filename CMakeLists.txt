cmake_minimum_required(VERSION 3.0)

project(Efs)

find_package(FLTK REQUIRED MODULE)
include(${FLTK_USE_FILE})

if(UNIX)
	find_package (Threads)
endif(UNIX)


set(Sources Efs.cpp)
set(Headers Efs.h)

#Efs
set(EFS_Lib_Sources EFS_Lib.cpp)
set(EFS_Lib_Headers EFS_Lib.h Common.h)
# 
add_library(EFS_Lib STATIC ${EFS_Lib_Sources} ${EFS_Lib_Headers})
#Bfs
set(BFS_Lib_Sources BFS_Lib.cpp)
set(BFS_Lib_Headers BFS_Lib.h Common.h)
#
add_library(BFS_Lib STATIC ${BFS_Lib_Sources} ${BFS_Lib_Headers})

add_executable(Efs ${Sources} ${Headers})
add_dependencies(Efs BFS_Lib EFS_Lib)
target_link_libraries(Efs fltk fltk_images EFS_Lib BFS_Lib) 

if(UNIX)
	target_link_libraries (Efs ${CMAKE_THREAD_LIBS_INIT})
endif(UNIX)

