#include "Common.h"

namespace Solver
{
	class BFS_Class :public Fs
	{
		int Rotate(char M, _Nd*, char);
	public:
		BFS_Class(int);
		~BFS_Class();

		// Inherited via Fs
		std::vector<std::vector<char>> FindSolution(std::unique_ptr<char[]>) override;
		int GetIteration() override;
		virtual int GetUsedListCount() override;
		void CancelTask() override;
	};
}
