#include "A_Star_Lib.h"


namespace Solver
{
	int EFS_Class::GetIteration()
	{
		return IterationCount;
	}
	short int EFS_Class::Calc(char value, short int position)
	{
		short int Dif = 0;
		short int xPositionIndex, yPositionIndex, xValueIndex, yValueIndex;
		xPositionIndex = position % ElementsInRow;
		yPositionIndex = position / ElementsInRow;

		xValueIndex = ((short int)value - 1) % ElementsInRow;
		yValueIndex = ((short int)value - 1) / ElementsInRow;

		Dif = std::abs(xValueIndex - xPositionIndex) + std::abs(yValueIndex - yPositionIndex);
		return Dif;
	}
	int EFS_Class::Rotate(char M, const _Nd *ParNode, char IsLeft)
	{
		IterationCount++;

		auto Tmp = std::make_unique<char[]>(ElementsInRow*ElementsInRow + 1);
		Tmp[ElementsInRow*ElementsInRow] = '\0';

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

		char* vl = Tmp.get();
		bool result;

		if (UsedList.find(vl) == UsedList.end())
		{
			auto _Node = std::make_unique<_Nd>();
			_Node->Parent = ParNode;
			_Node->Positions = std::move(Tmp);
			_Node->gValue = ParNode->gValue + 1;
			_Node->hValue = GetManhattan(_Node->Positions.get());
			_Node->fValue = _Node->gValue + _Node->hValue;
			//Insert combination to UsedList
			_Nd *TmpNode = _Node.get();

			std::tie(std::ignore, result) = UsedList.emplace(vl, std::move(_Node));

			auto itr = GreenQueue.insert(TmpNode);
		}
		else
		{
			return 1;
		}
		return 0;
	}


	short int EFS_Class::GetManhattan(char *Value)
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
		short int Sum = 0;
		for (short int i = 0; i < ElementsInRow*ElementsInRow; i++)
			Sum += Calc(Value[i], i);
		return Sum;
	}

	EFS_Class::~EFS_Class() {}
	EFS_Class::EFS_Class(int _n)
	{
		ElementsInRow = _n;
		IterationCount = 0;
		CancelationFlag = false;
	};
	std::vector<std::vector<char>> EFS_Class::FindSolution(std::unique_ptr<char[]> inData)
	{
		std::cout << "sizeof _Nd: " << sizeof(_Nd) << std::endl;
		try
		{
			const _Nd* currentNode;
			auto _Node = std::make_unique<_Nd>();

			inData[ElementsInRow*ElementsInRow] = '\0';
			_Node->Positions = std::move(inData);
			_Node->gValue = 0;
			_Node->hValue = GetManhattan(_Node->Positions.get());
			_Node->fValue = _Node->gValue + _Node->hValue;
			//Add to CloseList
			currentNode = _Node.get();
			UsedList.emplace(_Node->Positions.get(),std::move(_Node));//insert combination

			//StartRecursiveFunction
			while (CancelationFlag == false)
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
	int EFS_Class::GetUsedListCount()
	{
		return UsedList.size();
	}
	void EFS_Class::CancelTask()
	{
		CancelationFlag = true;
	}
}
