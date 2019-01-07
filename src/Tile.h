#ifndef TILE_H
#define TILE_H

#include "BoxesPreferences.h"

#include <string>

class Tile:public BoxesPreferences
{
 private:
	std::string str;
	char Data;
 public:
	static int Width_Height;
	static int FontSize;

	Tile() = default;
	Tile(int _x, int _y, int _h, int _w, Fl_Color, Fl_Fontsize fontSize, char _data);
	char GetData() const;
	void SetData(int);
	const char* GetRawData()const;
};
#endif
