#include "Common.h"

namespace Solver
{
	class BFS_Class :public Fs
	{
		int Rotate(char M, _Nd*, char);
	public:
		BFS_Class(int);
		std::vector<std::vector<char>> FindSolution(std::unique_ptr<char[]>) override;
		int GetIteration();
		~BFS_Class();
	};
}
