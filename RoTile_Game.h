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

static void TimerR(void*);//To preserve static
struct BoxesPreferences
{
	std::array<char, 2> str;
	int X;
	int Y;
	char Data;
	std::unique_ptr<Fl_Box> Box;
};
class Mybox : public Fl_Box
{
	Fs* m_fsClass;
	//EFS_Class m_efsClass;
	//BFS_Class m_bfsClass;
	std::future<std::vector<char*>> m_fut;

	BoxesPreferences* Tiles;
	BoxesPreferences Frame;
	int x, y;
	int Tile_Width_Height;
	int TilesInRow;
	std::vector<char> Solution;
	std::vector<char*> BackList;
public:
	Mybox(int _x, int _y, int _w, int _h, int elemsCount);
	~Mybox();
	friend void TimerR(void*);
	void draw();
	int handle(int e);
	void CheckSolution();
	BoxesPreferences* GetFrameLeftUpperPosition();
	void TurnRight_(BoxesPreferences* BX);
	void TurnLeft_(BoxesPreferences* BX);
	void *PrgsBar(void *ptr);
};

