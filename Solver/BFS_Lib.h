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
		int Rotate(char, std::shared_ptr<_Nd>, char) override;
		int GetIteration() const override;
		int GetUsedListCount() const override;
		int GetGreenListCount() const override;
		void CancelTask() override;
	};
}
