#include "A_Star_Lib.h"


namespace Solver
{
	int EFS_Class::GetIteration()
	{
		return IterationCount;
	}
	int EFS_Class::Calc(char value, int position)
	{
		int Dif = 0;
		int xPositionIndex, yPositionIndex, xValueIndex, yValueIndex;
		xPositionIndex = position % ElementsInRow;
		yPositionIndex = position / ElementsInRow;

		xValueIndex = ((int)value - 1) % ElementsInRow;
		//xValueIndex = (std::stoi(&value) - 1) % ElementsInRow;
		yValueIndex = ((int)value - 1) / ElementsInRow;
		//yValueIndex = (std::stoi(&value) - 1) / ElementsInRow;

		Dif = std::abs(xValueIndex - xPositionIndex) + std::abs(yValueIndex - yPositionIndex);
		return Dif;
	}
	int EFS_Class::Rotate(char M, _Nd *ParNode, char IsLeft)
	{
		IterationCount++;

		_Nd* _Node = new _Nd;
		//_Nd *TmpNode = _Node;
		if (_Node == nullptr)std::cout << "memory exhausted\n";
		//std::memset(Node,0,sizeof(_Nd));
		_Node->Parent = ParNode;
		auto Tmp = std::make_unique<char[]>(ElementsInRow*ElementsInRow + 1);
		if (Tmp == nullptr)std::cout << "memory exhausted\n";
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
		Tmp.get()[ElementsInRow*ElementsInRow + 1] = '\0';
		_Node->Positions = std::move(Tmp);
		_Node->gValue = ParNode->gValue + 1;
		_Node->hValue = GetManhattan(_Node->Positions.get());
		_Node->fValue = _Node->gValue + _Node->hValue;
		//Insert combination to UsedList
		bool result;
		std::tie(std::ignore, result) = UsedList.insert(_Node->Positions.get());
		//Check existance in UsedList
		if (result == true || _Node->hValue == 0)
		{
			//auto v = GreenQueue.lower_bound(_Node);
			//if (v == GreenQueue.begin()
			//	&& v != GreenQueue.end()
			//	&& v.operator*()->fValue == _Node->fValue
			//	&& v.operator*()->hValue > _Node->hValue)
			//	GreenQueue.emplace_hint(v, std::move(_Node));
			//else
			GreenQueue.insert(std::move(_Node));
		}
		else
		{
			return 1;
		}
		return 0;
	}


	int EFS_Class::GetManhattan(char *Value)
	{
		/*
		- State -
		675
		432
		198
		- Manhattan values-
		331
		022
		211
		- Sum -
		3+3+1+0+2+2+1+1=13
		*/
		int Sum = 0;
		for (int i = 0; i < ElementsInRow*ElementsInRow; i++)
			Sum += Calc(Value[i], i);
		return Sum;
	}

	EFS_Class::EFS_Class(int _n)
	{
		ElementsInRow = _n;
		IterationCount = 0;
	};
	std::vector<std::vector<char>> EFS_Class::FindSolution(char *inData)
	{
		auto Comb = std::make_unique<char[]>(ElementsInRow*ElementsInRow + 1);
		try

		{
			Node = new _Nd;
			//_Nd *TmpNode = Node;
			std::memset(Node, 0, sizeof(_Nd));
			std::copy(inData, inData + ElementsInRow * ElementsInRow, Comb.get());
			Comb[ElementsInRow*ElementsInRow] = '\0';
			Node->Positions = std::move(Comb);
			Node->gValue = 0;
			Node->hValue = GetManhattan(Node->Positions.get());
			Node->fValue = Node->gValue + Node->hValue;
			//Add to CloseList
			UsedList.insert(Node->Positions.get());//insert combination

			//StartRecursiveFunction
			while (1)
			{
				for (char i = 0; i < (ElementsInRow - 1); i++)
				{
					for (char j = 0; j < (ElementsInRow - 1); j++)
					{
						for (char Left = 0; Left < 2; Left++)
						{
							//Creation
							/*Node->Child[xValueIndex] = */Rotate(ElementsInRow*(int)i + (int)j, Node, (bool)Left);
						}
					}
				}
				auto It = GreenQueue.begin();
				Node = *It;
				if (Node->hValue == 0)
				{

					while (Node)
					{
						BackList.push_back({ Node->Positions.get(),Node->Positions.get() + ElementsInRow * ElementsInRow + 1 });
						if (Node->Parent) Node = Node->Parent;
						else break;
					}
					return BackList;
				}
				RedQueue.push_back(*It);
				//std::move(GreenQueue.begin, std::next(it), std::back_inserter(RedQueue));
				GreenQueue.erase(It);
			}
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
		return std::vector<std::vector<char>>();
	}
}
