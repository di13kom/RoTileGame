#ifndef FRAME_H
#define FRAME_H
#include "BoxesPreferences.h"

class Frame:public BoxesPreferences
{
 public:
	Frame() = default;
	Frame(int _x, int _y, int _h, int _w, Fl_Color);
};
#endif
