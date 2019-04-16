#include "BFS_Lib.h"

namespace Solver
{
	int BFS_Class::GetIteration()
	{
		return IterationCount;
	}
	BFS_Class::BFS_Class(int _n)
	{
		ElementsInRow = _n;
		IterationCount = 0;
		Solution = std::make_unique<char[]>(ElementsInRow*ElementsInRow);
		for (int i = 0; i < ElementsInRow*ElementsInRow; i++)
		{
			//Solution.push_back(i+1);
			Solution[i] = i + 1;
		}
	};

	BFS_Class::~BFS_Class() {}

	std::vector<std::vector<char>> BFS_Class::FindSolution(char* inData)
	{
		auto Comb = std::make_unique<char[]>(ElementsInRow*ElementsInRow);
		try
		{
			auto _Node = std::make_unique<_Nd>();
			for (int i = 0; i < ElementsInRow*ElementsInRow; i++)
			{
				Comb[i] = inData[i];
			}
			Comb[ElementsInRow*ElementsInRow] = '\0';
			_Node->Positions = std::move(Comb);
			UsedList.insert(std::move(_Node));
			//StartRecursiveFunction
			for (char i = 0; i < (ElementsInRow - 1); i++)
			{
				for (char j = 0; j < (ElementsInRow - 1); j++)
				{
					for (char Left = 0; Left < 2; Left++)
					{
						if (Rotate(ElementsInRow*(int)i + (int)j, _Node.get(), (bool)Left))
							return BackList;
					}
				}
			}
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
		return std::vector<std::vector<char>>();
	}

	int BFS_Class::Rotate(char M, _Nd *ParNode, char IsLeft)
	{
		_Nd* currentNode = nullptr;
		IterationCount++;
		auto _Node = std::make_unique<_Nd>();
		_Node->Parent = ParNode;
		auto Tmp = std::make_unique<char[]>(ElementsInRow*ElementsInRow);
		std::copy(ParNode->Positions.get(), ParNode->Positions.get() + ElementsInRow * ElementsInRow, Tmp.get());
		if (IsLeft)//Left Rotate
		{
			std::swap(Tmp[(int)M + 1], Tmp[(int)M]);
			std::swap(Tmp[(int)M + 1], Tmp[(int)M + ElementsInRow]);
			std::swap(Tmp[(int)M + 1], Tmp[(int)M + ElementsInRow + 1]);
		}
		else//Right Rotate
		{
			std::swap(Tmp[(int)M], Tmp[(int)M + 1]);
			std::swap(Tmp[(int)M], Tmp[(int)M + ElementsInRow + 1]);
			std::swap(Tmp[(int)M], Tmp[(int)M + ElementsInRow]);
		}
		Tmp[ElementsInRow*ElementsInRow] = '\0';
		_Node->Positions = std::move(Tmp);
		if (std::equal(Tmp.get(), Tmp.get() + (ElementsInRow*ElementsInRow/*(ElementsInRow-1)*/), Solution.get()))
		{
			//std::cout<<"match with ideal\n";
			while (currentNode)
			{
				BackList.push_back({ currentNode->Positions.get(), currentNode->Positions.get() + ElementsInRow * ElementsInRow + 1 });
				if (_Node->Parent) currentNode = currentNode->Parent;
				else break;
				//std::cout<<"ura\n";
			}
			return 1;
		}
		else
		{
			auto vl = UsedList.insert(std::move(_Node));
			if (vl.second == true)
			{
				for (char i = 0; i < (ElementsInRow - 1); i++)
				{
					for (char j = 0; j < (ElementsInRow - 1); j++)
					{
						for (char Left = 0; Left < 2; Left++)
						{
							if (Rotate(ElementsInRow*(char)i + (char)j, _Node.get(), (bool)Left))
								return 1;
							//Rotate(ElementsInRow*i+j, Comb, 0);
						}
					}
				}
			}
			return 0;
		}
	}
}
