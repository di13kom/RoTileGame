#include "BFS_Lib.h"

namespace Solver
{
	int BFS_Class::GetIteration() const
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

	int BFS_Class::GetUsedListCount() const
	{
		return UsedList.size();
	}

	int BFS_Class::GetGreenListCount() const
	{
		return 0;
	}

	void BFS_Class::CancelTask()
	{
	}

	std::vector<std::vector<char>> BFS_Class::FindSolution(std::unique_ptr<char[]> inData)
	{
		try
		{
			auto _Node = std::make_shared<_Nd>();
			inData[ElementsInRow*ElementsInRow] = '\0';
			_Node->Positions = std::move(inData);
			UsedList.insert(_Node->Positions);
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

	int BFS_Class::Rotate(char M, std::shared_ptr<_Nd> ParNode, char IsLeft)
	{
		std::shared_ptr<_Nd> currentNode = nullptr;
		IterationCount++;
		auto _Node = std::make_shared<_Nd>();
		_Node->Parent = ParNode;
		std::shared_ptr<char[]> Tmp(new char[ElementsInRow*ElementsInRow + 1], std::default_delete<char[]>());
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
			auto vl = UsedList.insert(_Node->Positions);
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
