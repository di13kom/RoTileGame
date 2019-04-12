#include "Common.h"

namespace Solver
{
	class BFS_Class :public Fs
	{
		//int ElementsInRow, IterationCount;
		//_Nd *Node;
		//std::vector<char> Solution;
		//std::vector<char*> BackList;
		//std::set<const char*, Comp2Func> UsedList;//Checking list for used combinations
		char* Solution;
		int Rotate(char M, _Nd*, char);
		public:
		BFS_Class(int);
		std::vector<std::vector<char>> FindSolution(char*);
		int GetIteration();
		~BFS_Class();
	};
}
