#include "A_Star_Lib.h"


namespace Solver
{
	int EFS_Class::GetIteration() const
	{
		return IterationCount;
	}

	int EFS_Class::GetGreenListCount() const
	{
		return GreenQueue.size();
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
	int EFS_Class::Rotate(char M, std::shared_ptr<_Nd> ParNode, char IsLeft)
	{
		//std::cout<<"Iter: "<<
		       	IterationCount++;

		std::shared_ptr<char[]> Tmp(new char[ElementsInRow*ElementsInRow + 1], std::default_delete<char[]>());
		//Tmp[ElementsInRow*ElementsInRow] = '\0';

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

		auto localhValue = GetManhattan(Tmp.get());
		auto localgValue = ParNode->gValue + 1;
		auto localfValue = localhValue + localgValue;

		auto itr = GreenQueue.rbegin();
		auto nd = *itr;
		if(localfValue < nd->fValue)
		{
			auto _Node = std::make_shared<_Nd>();
			_Node->Parent = ParNode;
			_Node->Positions = Tmp;
			_Node->gValue = localgValue;
			_Node->hValue = localhValue;
			_Node->fValue = localfValue;
			//Insert combination to UsedList
			//_Nd *TmpNode = _Node.get();

			bool result;
			std::tie(std::ignore, result) = UsedList.insert(Tmp);
			//Check existance in UsedList
			if (result == true || _Node->hValue == 0)
			{
				//auto v = GreenQueue.lower_bound(_Node);
				//if (v == GreenQueue.begin()
				//	&& v != GreenQueue.end()
				//	&& v->fValue == _Node->fValue
				//	&& v->hValue > _Node->hValue)
				//	GreenQueue.emplace_hint(v, std::move(_Node));
				//else
				GreenQueue.insert(_Node);

#ifdef STORAGE_REQUIRED
				std::ostringstream memStr;
				for(int i=0;i<ElementsInRow*ElementsInRow;i++)
				{
					memStr<<(int)TmpNode->Positions[i];
					if(i<ElementsInRow*ElementsInRow-1)
						memStr<<",";
				}
				StorageFile<<"{\"Node\": "<<TmpNode
					<<",\"ParNode\": "<<TmpNode->Parent
					<<",\"gValue\": "<<TmpNode->gValue
					<<",\"hValue\": "<<TmpNode->hValue
					<<",\"fValue\": "<<TmpNode->fValue
					<<",\"P\": "<<memStr.str()
					<<"},\n";
#endif
				return 0;
			}
		}
		return 1;
	}


	constexpr short int EFS_Class::GetManhattan(char *Value)
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
		std::cout<<"sizeof _Nd: "<<sizeof(_Nd)<<std::endl;
		try
		{
#ifdef STORAGE_REQUIRED
			StorageFile.open("solv.json",std::fstream::in|std::fstream::out|std::fstream::trunc);
			StorageFile<<"[\n";
#endif
			std::shared_ptr<_Nd> currentNode;
			auto _Node = std::make_shared<_Nd>();

			//
			inData[ElementsInRow*ElementsInRow] = '\0';
			//_Node->Positions = std::make_shared<char[]>(ElementsInRow*ElementsInRow + 1);
			_Node->Positions = std::shared_ptr<char[]>(new char[ElementsInRow*ElementsInRow + 1], std::default_delete<char[]>());
			_Node->Positions[ElementsInRow*ElementsInRow] = '\0';

			std::copy(inData.get(), inData.get() + ElementsInRow * ElementsInRow, _Node->Positions.get());
			//
			_Node->gValue = 0;
			_Node->hValue = GetManhattan(_Node->Positions.get());
			_Node->fValue = _Node->gValue + _Node->hValue;
			//Add to CloseList
			currentNode = _Node;
			UsedList.insert(_Node->Positions);//insert combination
			GreenQueue.insert(_Node);

			//StartRecursiveFunction
			while (CancelationFlag == false)
			{
				for (char i = 0; i < (ElementsInRow - 1); i++)
				{
					for (char j = 0; j < (ElementsInRow - 1); j++)
					{
						for (char Left = 0; Left < 2; Left++)
						{
							Rotate(ElementsInRow*(int)i + (int)j, currentNode, (bool)Left);
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
#ifdef STORAGE_REQUIRED
					StorageFile<<"]\n";
					StorageFile.close();
#endif
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
	int EFS_Class::GetUsedListCount() const
	{
		return UsedList.size();
	}
	void EFS_Class::CancelTask()
	{
		CancelationFlag = true;
	}
}
