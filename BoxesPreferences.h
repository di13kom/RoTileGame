#ifndef BOXESPREFERENCES_H
#define BOXESPREFERENCES_H
#include <iostream>
#include <array>
#include <FL/Fl_Box.H>
class BoxesPreferences
{
	//std::array<char, 2> str;
	char *str;
	int X;
	int Y;
	char Data;
	std::unique_ptr<Fl_Box> Box;
public:
	BoxesPreferences() = default;
	BoxesPreferences(int _x, int _y, int _h, int _w);
	BoxesPreferences(int _x, int _y, int _h, int _w, double fontSize, char _data);
	//~BoxesPreferences();
	void SetX(int _x);
	int GetX();
	void SetY(int _y);
	int GetY();
	char GetData();
	void SetData(int);
	void SetColor(Fl_Color);
};

#endif