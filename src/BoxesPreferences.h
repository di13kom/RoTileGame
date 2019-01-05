#ifndef BOXESPREFERENCES_H
#define BOXESPREFERENCES_H
#include <iostream>
#include <FL/Fl_Box.H>
class BoxesPreferences
{
 protected:
	int X;
	int Y;
	Fl_Color Color;
 public:
	BoxesPreferences() = default;
	//~BoxesPreferences();
	void SetX(int _x);
	int GetX() const;
	void SetY(int _y);
	int GetY() const;
	Fl_Color GetColor() const;
	void SetColor(const Fl_Color& clr);
};

#endif
