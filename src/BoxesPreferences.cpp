#include "BoxesPreferences.h"

void BoxesPreferences::SetX(int _x)
{
	X = _x;
}

int BoxesPreferences::GetX() const
{
	return X;
}

void BoxesPreferences::SetY(int _y)
{
	Y = _y;
}

int BoxesPreferences::GetY() const
{
	return Y;
}


Fl_Color BoxesPreferences::GetColor() const
{
	return Color;
}

void BoxesPreferences::SetColor(const Fl_Color& clr)
{
	Color = clr;
}
