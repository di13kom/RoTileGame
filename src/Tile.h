#ifndef TILE_H
#define TILE_H

#include "BoxesPreferences.h"

#include <string>

class Tile:public BoxesPreferences
{
 private:
	std::string str;
	char Data;
	bool IsBorderTile;
 public:
	static int Width_Height;
	static int FontSize;

	Tile() = default;
	Tile(int, int, int, int, bool, Fl_Color, Fl_Fontsize fontSize, char);
	char GetData() const;
	void SetData(int);
	const char* GetRawData()const;
	void SetIsBorderTile(bool);
	bool GetIsBorderTile() const;
};
#endif
