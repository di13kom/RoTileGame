#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Button.H>
#include <ctime>        // std::time
#include <cstdlib>
#include <signal.h>
#include "BFS_Lib.h"
#include "A_Star_Lib.h"
#include <thread>
#include <future>
#include <array>
#include <random>
#include "BoxesPreferences.h"

static void TimerR(void*);//To preserve static

static int s_FramePadding = 5;
static int s_InterTileDistance = 10;
static int s_MainTablePadding = 35;
static int s_BasicFontSize = 40;


class Mybox : public Fl_Box
{
	Fs* m_fsClass;
	//EFS_Class m_efsClass;
	//BFS_Class m_bfsClass;
	std::future<std::vector<char*>> m_fut;

	std::unique_ptr<BoxesPreferences>Frame;
	int m_MainTableXpos, m_MainTableYpos;
	int Tile_Width_Height;
	int TilesInRow;
	std::vector<BoxesPreferences> Tiles;
	std::vector<char> Solution;
	std::vector<char*> BackList;
public:
	Mybox(Fl_Boxtype bt, int _x, int _y, int _w, int _h, int elemsCount);
	~Mybox();
	friend void TimerR(void*);
	void draw();
	int handle(int e);
	void CheckSolution();
	int GetFrameLeftUpperPosition();
	void TurnRight_(int bxInd);
	void TurnLeft_(int bxInd);
	void *PrgsBar(void *ptr);
	void SetFramPositionByTileIndex(int);
};

