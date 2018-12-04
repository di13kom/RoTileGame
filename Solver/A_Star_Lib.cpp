#include "A_Star_Lib.h"


int EFS_Class::Calc(char Num, int Pos)
{
	int Dif = 0;
	int x1, y1, x, y;
	x1 = Pos % N;
	y1 = Pos / N;

	x = ((int)Num - 1) % N;
	//x = (std::stoi(&Num) - 1) % N;
	y = ((int)Num - 1) / N;
	//y = (std::stoi(&Num) - 1) / N;

	Dif = std::abs(x - x1) + std::abs(y - y1);
	return Dif;
}
int EFS_Class::Rotate(char M, char* Output, _Nd *ParNode, char IsLeft)
{
	L++;

	_Nd* _Node = new _Nd;
	//_Nd *TmpNode = _Node;
	if (_Node == nullptr)std::cout << "memory exhausted\n";
	//std::memset(Node,0,sizeof(_Nd));
	_Node->Parent = ParNode;
	//bool NotEqualFlag = false;
	char *Tmp = new char[N*N + 1];
	if (Tmp == nullptr)std::cout << "memory exhausted\n";
	std::copy(Output, Output + N * N, Tmp);
	char *Buff = new char;
	if (IsLeft)//Left Rotate
	{
		*Buff = Tmp[(int)M];
		Tmp[(int)M] = Tmp[(int)M + 1];
		Tmp[(int)M + 1] = Tmp[(int)M + N + 1];
		Tmp[(int)M + N + 1] = Tmp[(int)M + N];
		Tmp[(int)M + N] = *Buff;
	}
	else//Right Rotate
	{
		*Buff = Tmp[(int)M];
		Tmp[(int)M] = Tmp[(int)M + N];
		Tmp[(int)M + N] = Tmp[(int)M + N + 1];
		Tmp[(int)M + N + 1] = Tmp[(int)M + 1];
		Tmp[(int)M + 1] = *Buff;
	}
	delete Buff;
	Tmp[N*N] = '\0';
	_Node->Positions = Tmp;
	_Node->gValue = ParNode->gValue + 1;
	_Node->hValue = GetManhattan(_Node->Positions);
	_Node->fValue = _Node->gValue + _Node->hValue;
	/**/
	/**/
	//Insert combination to UsedList
	//size_t sz;
	//sz = UsedList.size();
	bool result;
	std::tie(std::ignore, result) = UsedList.insert(_Node->Positions);
	//Check existance in UsedList
	//if(sz != UsedList.size() || _Node->hValue == 0)
	if (result == true || _Node->hValue == 0)
	{
		Queue.insert(_Node);
	}
	else
	{
		delete _Node->Positions;
		delete _Node;
		return 1;
	}
	return 0;
}


int EFS_Class::GetManhattan(char *Value)
{
	int Sum = 0;
	for (int i = 0; i < N*N; i++)
		Sum += Calc(Value[i], i);
	return Sum;
}

EFS_Class::EFS_Class(int _n)
{
	N = _n;
	L = 0;
};
std::vector<char*> EFS_Class::FindSolution(char *inData)
{
	char *Comb = new char[N*N + 1];
	Node = new _Nd;
	//_Nd *TmpNode = Node;
	std::memset(Node, 0, sizeof(_Nd));
	for (int i = 0; i < N*N; i++)
	{
		Comb[i] = inData[i];
		//Comb[i] = BoxPref[i].Data;
		//std::cout<<(int) Comb[i]<<std::endl;
	}
	Comb[N*N] = '\0';
	Node->Positions = Comb;
	Node->gValue = 0;
	Node->hValue = GetManhattan(Node->Positions);
	Node->fValue = Node->gValue + Node->hValue;
	//Add to CloseList
	UsedList.insert(Node->Positions);//insert combination

	// Create child


	//StartRecursiveFunction
	while (1)
	{
		for (char i = 0; i < (N - 1); i++)
		{
			for (char j = 0; j < (N - 1); j++)
			{
				for (char Left = 0; Left < 2; Left++)
				{
					//Creation
					/*Node->Child[x] = */Rotate(N*(int)i + (int)j, Node->Positions, Node, (bool)Left);
				}
			}
		}
		//auto It = std::min_element(Queue.begin(),Queue.end(),Comp1Func()); 
		auto It = Queue.begin();
		Node = *It;
		if (Node->hValue == 0)
		{

			//if(std::equal(Tmp, Tmp+(N*N), Solution))
			//{
			//std::cout<<"match with ideal\n";
			while (Node)
			{
				BackList.push_back(Node->Positions);
				if (Node->Parent) Node = Node->Parent;
				else break;
				//std::cout<<"ura\n";
			}
			//Calculated = 1;
			//return 1;
			return BackList;
		}
		Queue.erase(It);
	}
	return std::vector<char*>();
}
