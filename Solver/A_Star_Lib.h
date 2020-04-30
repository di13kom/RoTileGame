#include "Common.h"
#include <fstream>
#include <sstream>

namespace Solver
{
	struct Comp1Func
	{
		bool operator()(const std::shared_ptr<_Nd>& s1, const std::shared_ptr<_Nd>& s2) const
		{
			return s1->fValue < s2->fValue;
		}
	};

	class EFS_Class :public Fs
	{
		std::multiset<std::shared_ptr<_Nd>, Comp1Func> GreenQueue;//Checking list for used combinations
		short int Calc(char, short int);
		int Rotate(char, std::shared_ptr<_Nd>, char) override;
		constexpr short int GetManhattan(char*);
		std::fstream StorageFile;
	public:
		EFS_Class(int);
		~EFS_Class();

		// Inherited via Fs
		std::vector<std::vector<char>> FindSolution(std::unique_ptr<char[]>) override;
		int GetIteration() const override;
		int GetUsedListCount() const override;
		int GetGreenListCount() const override;
		void CancelTask() override;
	};
}
