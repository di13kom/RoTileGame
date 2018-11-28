#include "Common.h"

class EFS_Class :public Fs
{
	//int N, L;
	//_Nd *Node;
	//std::set<const char*, Comp2Func> UsedList;//Checking list for used combinations
	//std::vector<char*> BackList;
	std::multiset<_Nd *, Comp1Func> Queue;//Checking list for used combinations
	int Calc(char, int);
	int Rotate(char, char*, _Nd*, char);
	int GetManhattan(char*);
public:
	EFS_Class(int);
	std::vector<char*> FindSolution(char*);
};