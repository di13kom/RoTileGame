#include "Common.h"

BFS_Class::BFS_Class(int _n)
{
	N = _n;
	L = 0;
	//Solution.reserve(N*N);
	Solution = new char[N*N];
	for(int i=0;i<N*N;i++)
	{
		//Solution.push_back(i+1);
		Solution[i]=i+1;
	}
};

BFS_Class::~BFS_Class()
{
		delete[] Solution;
}

std::vector<char*> BFS_Class::FindSolution(char* inData)
{
	char *Comb = new char[N*N+1];
	Node = new _Nd;
	_Nd *TmpNode = Node;
	std::memset(Node,0,sizeof(_Nd));
	for(int i=0;i<N*N;i++)
	{
		Comb[i] = inData[i];
	}
	Comb[N*N] = '\0';
	Node->Positions = Comb;
	UsedList.insert(Comb);
	//StartRecursiveFunction
	for(char i=0; i<(N-1);i++)
	{
		for(char j=0; j<(N-1);j++)
		{
			for(char Left=0;Left<2;Left++)
			{
				Node = TmpNode;
				if(Rotate(N*(int)i+(int)j, Comb, Node, (bool)Left))
					return BackList;
			}
		}
	}
	return std::vector<char*>();
}

int BFS_Class::Rotate(char M, char* Output, _Nd *ParNode, char IsLeft)
{
	L++;
	Node = new _Nd;
	_Nd *TmpNode = Node;
	if(Node == NULL)std::cout<<"memory exhausted\n";
	Node->Parent = ParNode;
	char *Tmp = new char[N*N+1];
	if(Tmp == NULL)std::cout<<"memory exhausted\n";
	std::copy(Output, Output+N*N, Tmp);
	char *Buff = new char;
	if(IsLeft)//Left Rotate
	{
		*Buff = Tmp[(int)M];
		Tmp[(int)M] = Tmp[(int)M+1];
		Tmp[(int)M+1] = Tmp[(int)M+N+1];
		Tmp[(int)M+N+1] = Tmp[(int)M+N];
		Tmp[(int)M+N] = *Buff;
	}
	else//Right Rotate
	{
		*Buff = Tmp[(int)M];
		Tmp[(int)M] = Tmp[(int)M+N];
		Tmp[(int)M+N] = Tmp[(int)M+N+1];
		Tmp[(int)M+N+1] = Tmp[(int)M+1];
		Tmp[(int)M+1] = *Buff;
	}
	delete Buff;
	Tmp[N*N]='\0';
	Node->Positions = Tmp;
	if(std::equal(Tmp, Tmp+(N*N/*(N-1)*/), Solution))
	{
		//std::cout<<"match with ideal\n";
		while(Node)
		{
			BackList.push_back(Node->Positions);
			if(Node->Parent) Node = Node->Parent;
			else break;
			//std::cout<<"ura\n";
		}
		return 1;
	}
	else
	{
		size_t sz;
		sz = UsedList.size();
		UsedList.insert(Tmp);
		if(sz != UsedList.size())
		{
			for(char i=0; i<(N-1);i++)
			{
				for(char j=0; j<(N-1);j++)
				{
					for(char Left=0;Left<2;Left++)
					{
						Node = TmpNode;
						if(Rotate(N*(char)i+(char)j, Tmp, Node, (bool)Left))
							return 1;
						//Rotate(N*i+j, Comb, 0);
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
