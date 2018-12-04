#include "Common.h"

class BFS_Class :public Fs
{
	//int N, L;
	//_Nd *Node;
	//std::vector<char> Solution;
	//std::vector<char*> BackList;
	//std::set<const char*, Comp2Func> UsedList;//Checking list for used combinations
	char* Solution;
	int Rotate(char M, char*, _Nd*, char);
public:
	BFS_Class(int);
	std::vector<char*> FindSolution(char*);
	~BFS_Class();
};