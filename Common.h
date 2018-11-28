#include <iostream>
#include <cstring>
#include <unistd.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Menu_Bar.H>
#include <algorithm>
#include <vector>
#include <ctime>        // std::time
#include <cstdlib>
#include <pthread.h>
#include <signal.h>
#include <string>
#include <iomanip> // std::setw
#include <memory>
#include <set>
#include <future>
#include <numeric> //iota

struct BoxesPreferences
{
	std::array<char,2> str;
	int X;
	int Y;
	char Data;
	std::unique_ptr<Fl_Box> Box;
};

struct _Nd
{
	_Nd* Parent;
	int hValue;
	int gValue;
	int fValue;
	char* Positions;
	_Nd** Child;
};


///-----------
struct Comp1Func
{
	bool operator()(const _Nd *s1, const _Nd *s2) const
	{
		return s1->fValue < s2->fValue;
	}
};

struct Comp2Func
{
	bool operator()(const char *s1, const char *s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};
class EFS_Class
{
	int N,L;
	_Nd *Node;
	std::multiset<_Nd *,Comp1Func> Queue;//Checking list for used combinations
	std::set<const char*,Comp2Func> UsedList;//Checking list for used combinations
	std::vector<char*> BackList;
	int Calc(char, int);
	int Rotate(char, char*, _Nd*, char);
	int GetManhattan(char*);
	public:
	EFS_Class(int);
	std::vector<char*> FindSolution(char*);
};

class BFS_Class
{
	int N,L;
	//std::vector<char> Solution;
	char* Solution;
	std::set<const char*,Comp2Func> UsedList;//Checking list for used combinations
	_Nd *Node;
	std::vector<char*> BackList;
	int Rotate(char M, char* , _Nd*, char);
	public:
	BFS_Class(int);
	std::vector<char*> FindSolution(char*);
	~BFS_Class();
};
//void *PrgsBar(void *ptr);
