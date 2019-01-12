#include "Tile.h"

Tile::Tile(int _x, int _y, int _h, int _w, bool isVertB, Fl_Color clr, Fl_Fontsize fontSize, char _data)
{
	SetData(_data);
	X = _x;
	Y = _y;
	IsBorderTile = isVertB;
	Color = clr;
	//std::cout<<"A: "<<std::addressof(str)<<std::endl;
}

char Tile::GetData() const
{
	return Data;
}

void Tile::SetData(int _data)
{
	Data = _data;
	str = std::to_string(_data);
	//std::cout<<"Sd: "<<std::addressof(str)<<std::endl;
}

const char* Tile::GetRawData() const
{
	return str.c_str();
}
void Tile::SetIsBorderTile(bool vl)
{
	IsBorderTile = vl;
}
bool Tile::GetIsBorderTile() const
{
	return IsBorderTile;
}
