#include "BoxesPreferences.h"

BoxesPreferences::BoxesPreferences(int _x, int _y, int _h, int _w)
{
	X = _x;
	Y = _y;
	Box = std::make_unique<Fl_Box>(FL_PLASTIC_UP_FRAME, X, Y, _h, _w, nullptr);
	Box->color(FL_RED);
}

BoxesPreferences::BoxesPreferences(int _x, int _y, int _h, int _w, double fontSize, char _data)
{
	str = new char[2];
	SetData(_data);
	X = _x;
	Y = _y;
	Box = std::make_unique<Fl_Box>(FL_PLASTIC_UP_BOX, X, Y, _h, _w, str);
	Box->labeltype(_FL_SHADOW_LABEL);
	Box->color(FL_GREEN);
	Box->labelfont(FL_HELVETICA_BOLD);
	Box->labelsize(fontSize);
}

//BoxesPreferences::~BoxesPreferences()
//{
//	delete[] str;
//}

void BoxesPreferences::SetX(int _x)
{
	X = _x;
	Box->position(X, Y);
}

int BoxesPreferences::GetX()
{
	return X;
}

void BoxesPreferences::SetY(int _y)
{
	Y = _y;
	Box->position(X, Y);
}

int BoxesPreferences::GetY()
{
	return Y;
}

char BoxesPreferences::GetData()
{
	return Data;
}

void BoxesPreferences::SetData(int _data)
{
	Data = _data;
	sprintf(str, "%d", Data);
	//sprintf(str.data(), "%d", Data);
	//Box->label(str.data());
}

void BoxesPreferences::SetColor(Fl_Color color)
{
	Box->color(color);
}
