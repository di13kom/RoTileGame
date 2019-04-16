#include "Common.h"

namespace Solver
{
	class EFS_Class :public Fs
	{
		std::multiset<_Nd *, Comp1Func> GreenQueue;//Checking list for used combinations
		int Calc(char, int);
		int Rotate(char, _Nd*, char);
		int GetManhattan(char*);
	public:
		EFS_Class(int);
		~EFS_Class();
		int GetIteration();
		std::vector<std::vector<char>> FindSolution(char*);
	};
}
