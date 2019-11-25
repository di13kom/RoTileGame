cmake_minimum_required(VERSION 3.0)


set(Sources RoTile_Game.cpp BoxesPreferences.cpp Tile.cpp Frame.cpp)
set(Headers RoTile_Game.h BoxesPreferences.h GameStateEnum.h Tile.h Frame.h)

if(MSVC_IDE)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RoTile_Game)
endif(MSVC_IDE)
add_executable(RoTile_Game ${Sources} ${Headers})
add_dependencies(RoTile_Game Solver_Lib)
target_include_directories(RoTile_Game PRIVATE ${Solver_Dir})

target_link_libraries(RoTile_Game fltk fltk_images Solver_Lib) 

if(UNIX)
	target_link_libraries (RoTile_Game ${CMAKE_THREAD_LIBS_INIT})
endif(UNIX)
