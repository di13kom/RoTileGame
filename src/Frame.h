#ifndef FRAME_H
#define FRAME_H
#include "BoxesPreferences.h"

class Tile;

class Frame : public BoxesPreferences
{
private:
	std::vector<Tile>::iterator UpperLeftTile;
	std::vector<Tile>::iterator UpperRightTile;
	std::vector<Tile>::iterator LowerLeftTile;
	std::vector<Tile>::iterator LowerRightTile;
 public:
	int static Width_Height;

	Frame() = default;
	Frame(int _x, int _y, int _h, int _w, Fl_Color);
};
#endif
