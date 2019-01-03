#ifndef GAMESTATEENUM_H
#define GAMESTATEENUM_H
#include <iostream>

enum class GameStateEnum
{
	Empty = 0,
	Initialization = 1,
	Normal = 2,
	FrameDragging = 3,
	TileDragging = 4,
	FindSolving = 5,
	Solved = 6,
};
#endif
