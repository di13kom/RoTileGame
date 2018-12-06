#include "BoxesPreferences.h"

BoxesPreferences::BoxesPreferences(int _x, int _y, int _h, int _w)
{
	X = _x;
	Y = _y;
	//Box = std::make_unique<Fl_Box>(FL_PLASTIC_UP_FRAME, X, Y, _h, _w, nullptr);
}

BoxesPreferences::BoxesPreferences(int _x, int _y, int _h, int _w, Fl_Fontsize fontSize, char _data)
{
	SetData(_data);
	X = _x;
	Y = _y;
	//std::cout<<"A: "<<std::addressof(str)<<std::endl;
}

void BoxesPreferences::SetX(int _x)
{
	X = _x;
}

int BoxesPreferences::GetX()
{
	return X;
}

void BoxesPreferences::SetY(int _y)
{
	Y = _y;
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
	str = std::to_string(_data);
	//std::cout<<"Sd: "<<std::addressof(str)<<std::endl;
}

const char* BoxesPreferences::GetRawData()
{
	return str.c_str();
}
