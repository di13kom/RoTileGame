#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Menu_Bar.H>
#include <ctime>        // std::time
#include <cstdlib>
#include <signal.h>
#include "BFS_Lib.h"
#include "A_Star_Lib.h"
#include <thread>
#include <future>
#include <array>

struct BoxesPreferences
{
	std::array<char, 2> str;
	int X;
	int Y;
	char Data;
	std::unique_ptr<Fl_Box> Box;
};
