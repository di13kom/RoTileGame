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

		auto _Node = std::make_unique<_Nd>();
		auto Tmp = std::make_unique<char[]>(ElementsInRow*ElementsInRow + 1);
		Tmp[ElementsInRow*ElementsInRow] = '\0';

		_Node->Parent = ParNode;
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
		_Node->Positions = std::move(Tmp);
		_Node->gValue = ParNode->gValue + 1;
		_Node->hValue = GetManhattan(_Node->Positions.get());
		_Node->fValue = _Node->gValue + _Node->hValue;
		//Insert combination to UsedList
		bool result;
		_Nd *TmpNode = _Node.get();
		std::tie(std::ignore, result) = UsedList.insert(std::move(_Node));
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
			GreenQueue.insert(TmpNode);
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
			_Nd* currentNode;
			auto _Node = std::make_unique<_Nd>();

			std::copy(inData, inData + ElementsInRow * ElementsInRow, Comb.get());
			Comb[ElementsInRow*ElementsInRow] = '\0';
			_Node->Positions = std::move(Comb);
			_Node->gValue = 0;
			_Node->hValue = GetManhattan(_Node->Positions.get());
			_Node->fValue = _Node->gValue + _Node->hValue;
			//Add to CloseList
			currentNode = _Node.get();
			UsedList.insert(std::move(_Node));//insert combination

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
							/*Node->Child[xValueIndex] = */Rotate(ElementsInRow*(int)i + (int)j, currentNode, (bool)Left);
						}
					}
				}
				auto It = GreenQueue.begin();
				currentNode = *It;
				if (currentNode->hValue == 0)
				{
					while (currentNode)
					{
						BackList.push_back({ currentNode->Positions.get(),currentNode->Positions.get() + ElementsInRow * ElementsInRow + 1 });
						if (currentNode->Parent) currentNode = currentNode->Parent;
						else break;
					}
					GreenQueue.clear();//Mandatory
					//UsedList.clear();//Skip
					return BackList;
				}
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
