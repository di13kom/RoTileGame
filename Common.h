#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip> // std::setw
#include <memory>
#include <set>
//#include <future>
#include <numeric> //iota
//#include <array>
//#include <thread>
#include <tuple>


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

class Fs
{
protected:
	int N, L;
	char* Solution;
	std::set<const char*, Comp2Func> UsedList;//Checking list for used combinations
	_Nd *Node;
	std::vector<char*> BackList;
	virtual int Rotate(char M, char*, _Nd*, char) = 0;
public:
	virtual std::vector<char*> FindSolution(char*) = 0;
	Fs(int);
	Fs() = default;
};

//void *PrgsBar(void *ptr);
#endif
