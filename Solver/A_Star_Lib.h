#include "Common.h"
#include<fstream>

namespace Solver
{
	class EFS_Class :public Fs
	{
		std::multiset<_Nd *, Comp1Func> GreenQueue;//Checking list for used combinations
		short int Calc(char, short int);
		int Rotate(char, _Nd*, char) override;
		constexpr short int GetManhattan(char*);
		std::fstream StorageFile;
	public:
		EFS_Class(int);
		~EFS_Class();

		// Inherited via Fs
		std::vector<std::vector<char>> FindSolution(std::unique_ptr<char[]>) override;
		int GetIteration() override;
		int GetUsedListCount() override;
		int GetGreenListCount() override;
		void CancelTask() override;
	};
}
