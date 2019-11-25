#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Return_Button.H>

#include "BFS_Lib.h"
#include "A_Star_Lib.h"
#include "BoxesPreferences.h"
#include "GameStateEnum.h"
#include "Tile.h"
#include "Frame.h"

#include <fstream>
#include <regex>
#include <signal.h>
#include <thread>
#include <future>
#include <array>
#include <random>
#include <algorithm>
#include <sstream>

using namespace Solver;

static void TimerR(void*);//To preserve static

static int s_FramePadding = 5;
static int s_InterTileDistance = 10;
static int s_MainTablePadding = 35;
static int s_BasicFontValue = 120;

char *s_FileName = nullptr;
//args
int helpFlag = 0;
int printSolutionFlag = 0;

struct ParseFileValue
{
	std::vector<int> Values;
	std::string ErrorMsg;
};

struct VisualDragTile
{
	std::vector<Tile>::iterator TileIterator;
	Tile VisualTile;
	bool IsLeftTile;
	bool IsUpperTile;
};

class Mybox : public Fl_Box
{
	std::future<std::vector<std::vector<char>>> m_fut;

	Frame m_Frame;
	Frame m_VisualDragFrame;
	VisualDragTile m_VisualDragTile;
	GameStateEnum GameState;
	int m_MainTableXpos, m_MainTableYpos, m_MainTableWidthHeight;
	int TilesInRow;
	std::vector<Tile> Tiles;
	std::vector<char> Solution;
	std::vector<std::vector<char>> BackList;

	friend void TimerR(void*);
	void draw();
	int handle(int e);
	void CheckSolution();
	int GetFrameLeftUpperPosition();
	void TurnRight(int bxInd);
	void TurnLeft(int bxInd);
	void *PrgsBar(void *ptr);
	static void CancelCallback(Fl_Widget*, void*);
	void SetFramePositionByTileIndex(int);
public:
	Mybox(Fl_Boxtype bt, int, int, int);
	~Mybox();
	void SetTilesValue(std::vector<int>);
};

