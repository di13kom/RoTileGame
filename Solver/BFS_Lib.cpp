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
		Solution = new char[ElementsInRow*ElementsInRow];
		for (int i = 0; i < ElementsInRow*ElementsInRow; i++)
		{
			//Solution.push_back(i+1);
			Solution[i] = i + 1;
		}
	};

	BFS_Class::~BFS_Class()
	{
		delete[] Solution;
	}

	std::vector<char*> BFS_Class::FindSolution(char* inData)
	{
		char *Comb = new char[ElementsInRow*ElementsInRow + 1];
		try
		{
			Node = new _Nd;
			_Nd *TmpNode = Node;
			std::memset(Node, 0, sizeof(_Nd));
			for (int i = 0; i < ElementsInRow*ElementsInRow; i++)
			{
				Comb[i] = inData[i];
			}
			Comb[ElementsInRow*ElementsInRow] = '\0';
			Node->Positions = Comb;
			UsedList.insert(Comb);
			//StartRecursiveFunction
			for (char i = 0; i < (ElementsInRow - 1); i++)
			{
				for (char j = 0; j < (ElementsInRow - 1); j++)
				{
					for (char Left = 0; Left < 2; Left++)
					{
						Node = TmpNode;
						if (Rotate(ElementsInRow*(int)i + (int)j, Node, (bool)Left))
							return BackList;
					}
				}
			}
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
		return std::vector<char*>();
	}

	int BFS_Class::Rotate(char M, _Nd *ParNode, char IsLeft)
	{
		IterationCount++;
		Node = new _Nd;
		_Nd *TmpNode = Node;
		if (Node == NULL)std::cout << "memory exhausted\n";
		Node->Parent = ParNode;
		char *Tmp = new char[ElementsInRow*ElementsInRow + 1];
		if (Tmp == NULL)std::cout << "memory exhausted\n";
		std::copy(ParNode->Positions, ParNode->Positions + ElementsInRow * ElementsInRow, Tmp);
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
		Node->Positions = Tmp;
		if (std::equal(Tmp, Tmp + (ElementsInRow*ElementsInRow/*(ElementsInRow-1)*/), Solution))
		{
			//std::cout<<"match with ideal\n";
			while (Node)
			{
				BackList.push_back(Node->Positions);
				if (Node->Parent) Node = Node->Parent;
				else break;
				//std::cout<<"ura\n";
			}
			return 1;
		}
		else
		{
			auto vl = UsedList.insert(Tmp);
			if (vl.second == true)
			{
				for (char i = 0; i < (ElementsInRow - 1); i++)
				{
					for (char j = 0; j < (ElementsInRow - 1); j++)
					{
						for (char Left = 0; Left < 2; Left++)
						{
							Node = TmpNode;
							if (Rotate(ElementsInRow*(char)i + (char)j, Node, (bool)Left))
								return 1;
							//Rotate(ElementsInRow*i+j, Comb, 0);
						}
					}
				}
			}
			else
			{
				delete Node;
				delete Tmp;
			}
			return 0;
		}
	}
}
