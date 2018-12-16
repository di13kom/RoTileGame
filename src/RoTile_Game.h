#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Return_Button.H>
#include <ctime>        // std::time
#include <cstdlib>
#include <signal.h>
#include "BFS_Lib.h"
#include "A_Star_Lib.h"
#include <thread>
#include <future>
#include <array>
#include <random>
#include <algorithm>
#include "BoxesPreferences.h"

static void TimerR(void*);//To preserve static

static int s_FramePadding = 5;
static int s_InterTileDistance = 10;
static int s_MainTablePadding = 35;
static int s_BasicFontSize = 40;


class Mybox : public Fl_Box
{
	//std::unique_ptr<Fs> m_fsClass;
	//EFS_Class m_efsClass;
	//BFS_Class m_bfsClass;
	std::future<std::vector<char*>> m_fut;

	BoxesPreferences Frame;
	BoxesPreferences VisualDraggingElement;
	bool IsInit;
	bool IsDragging;
	int m_MainTableXpos, m_MainTableYpos, m_MainTableWidth;
	int m_FontSize;
	int Tile_Width_Height;
	int Frame_Width_Height;
	int TilesInRow;
	std::vector<BoxesPreferences> Tiles;
	std::vector<char> Solution;
	std::vector<char*> BackList;

	friend void TimerR(void*);
	void draw();
	int handle(int e);
	void CheckSolution();
	int GetFrameLeftUpperPosition();
	void TurnRight_(int bxInd);
	void TurnLeft_(int bxInd);
	void *PrgsBar(void *ptr);
	void SetFramPositionByTileIndex(int);
public:
	Mybox(Fl_Boxtype bt, int _x, int _y, int _w, int _h);
	~Mybox();
	void SetTilesValue(int elemsCount);
};

