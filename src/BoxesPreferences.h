#ifndef BOXESPREFERENCES_H
#define BOXESPREFERENCES_H
#include <iostream>
#include <array>
#include <memory>
#include <FL/Fl_Box.H>
#include <string>
class BoxesPreferences
{
	std::string str;
	int X;
	int Y;
	char Data;
	Fl_Color Color;
public:
	BoxesPreferences() = default;
	BoxesPreferences(int _x, int _y, int _h, int _w, Fl_Color);
	BoxesPreferences(int _x, int _y, int _h, int _w, Fl_Color, Fl_Fontsize fontSize, char _data);
	//~BoxesPreferences();
	void SetX(int _x);
	int GetX() const;
	void SetY(int _y);
	int GetY() const;
	Fl_Color GetColor() const;
	void SetColor(const Fl_Color& clr);
	char GetData() const;
	void SetData(int);
	const char* GetRawData()const;
};

#endif
