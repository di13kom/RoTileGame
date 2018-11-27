#include <iostream>
#include <cstring>
#include <unistd.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Progress.H>
#include <algorithm>
#include <vector>
#include <set>
#include <ctime>        // std::time
#include <cstdlib>
#include <pthread.h>
#include <signal.h>
#include <string>
#include <iomanip> // std::setw

int L;
void Handler(int)
{
	std::cout<<"SigSigv cathced"<<std::endl;
	std::cout<<"Itetations: "<<L<<std::endl;
}

struct BoxesPreferences
{
	char *str;
	int X;
	int Y;
	char Data;
	Fl_Box *Box;
};

struct _Nd
{
	_Nd* Parent;
	int hValue;
	int gValue;
	int fValue;
	char* Positions;
	_Nd** Child;
};

struct Comp1Func
{
	bool operator()(const _Nd *s1, const _Nd *s2) const
	{
		return s1->fValue < s2->fValue;
	}
};

struct Comp2Func
{
	bool operator()(const char *s1, const char *s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};

Fl_Double_Window *wind;
void *PrgsBar(void *ptr);
static void TimerR(void* UserData);
pthread_t thrd;
bool Calculated;

class Mybox: public Fl_Box
{
	BoxesPreferences *BoxPref;
	BoxesPreferences FramePref;
	int x,y;
	int p;
	int HWBox;
	int N;
	int HWFrm;
	int FrmPosNum;
	int xc,yc;
	char *Numb;
	char *Solution;
	//FindSolutin
	//std::vector<_Nd *> Queue;//Checking list for used combinations
	std::multiset<_Nd *,Comp1Func> Queue;//Checking list for used combinations
	std::set<const char*,Comp2Func> UsedList;//Checking list for used combinations
	_Nd *Node;
	std::vector<_Nd*> BackList;
	//std::vector<char*> BackList;
public:

	friend void TimerR(void*);
	Mybox(int _x, int _y, int _w, int _h,int _n): Fl_Box(_x,_y,_w,_h)//{}
	{
		Calculated = false;
		N = _n;
		x = _x;
		y = _y;
		BoxPref = new BoxesPreferences[N*N];
		Numb = new char[N*N];
		Solution= new char[N*N];
		for(int i=0;i<N*N;i++)
		{
			Numb[i]=i+1;
			Solution[i]=i+1;
		}
		std::random_shuffle(Numb, Numb+(N*N));
		for(int i=0;i<N*N;i++)
		{
			//std::cout<<Numb[i]<<"\n";
			BoxPref[i].str = new char;
			BoxPref[i].Data = Numb[i];
			sprintf(BoxPref[i].str, "%d",BoxPref[i].Data);
		}
		delete[] Numb;
		p = 0;
		HWBox = (430-(N-1)*10)/N;
		HWFrm = HWBox*2 + 10*2;
		//std::cout<<"HWBox:"<<HWBox<<"\n";
		for(int i=0;i<N;i++)
		{
			for(int j=0;j<N;j++)
			{
				BoxPref[p].X = (x+40) + j*10 + j*HWBox;
				BoxPref[p].Y = (y+40) + i*10 + i*HWBox;
				p++;
			}
		}
		for(int i=0;i<N*N;i++)
		{
			BoxPref[i].Box = new Fl_Box(FL_PLASTIC_UP_BOX, BoxPref[i].X, BoxPref[i].Y, HWBox, HWBox, BoxPref[i].str);
			BoxPref[i].Box->color(FL_GREEN);
			BoxPref[i].Box->labelfont(FL_HELVETICA_BOLD);
			BoxPref[i].Box->labelsize(40);
		}

		/*FramePref.X = x + 145;
		FramePref.Y = y + 145;*/
		FrmPosNum = (((N-1)*(N-1))/2)+1;
		yc = FrmPosNum/(N-1);
		xc = (FrmPosNum%(N-1))-1;
		//std::cout<<"xc:"<<xc<<"\nyc:"<<yc<<"\n";
		FramePref.X = (x+35) + xc*HWBox + xc*10;
		FramePref.Y = (y+35) + yc*HWBox + yc*10;
		FramePref.Box= new Fl_Box(FL_PLASTIC_UP_FRAME, FramePref.X, FramePref.Y, HWFrm, HWFrm, "");
		FramePref.Box->color(FL_RED);
		signal(SIGUSR1,Handler);
	}

	void draw()
	{
		//Fl_Box::draw();
		//wind->redraw();
		//std::cout<<"REdraw called\n";
		//fl_frame("AAAA", FramePref.X, FramePref.Y, 220, 220);
		//fl_color(FL_RED);
		//Fl_Box(FL_PLASTIC_UP_BOX, 10, 10, HWBox, HWBox,BoxPref[15].str);
		//Frame->redraw();
		//Frame->resize(FramePref.X + 110, FramePref.Y, 20, 20);
		//redraw();
	}

	int handle(int e)
	{
		switch(e)
		{
			case FL_SHORTCUT:
				{
					switch(Fl::event_key())
					{
						case FL_Left:
							//std::cout<<"Left Pressed\n";
							FramePref.X -= HWBox+10;
							if(FramePref.X<x + 35)
								FramePref.X = (x+35) + (N-2)*10 + (N-2)*HWBox;
							FramePref.Box->position(FramePref.X, FramePref.Y);
							wind->redraw();
							//std::cout<<"FramePref.X="<<FramePref.X<<"\n";
							return 1;
						case FL_Right:
							//std::cout<<"Right Pressed\n";
							FramePref.X += HWBox+10;
							if(FramePref.X > ((x+35) + (N-2)*10 + (N-2)*HWBox))
								FramePref.X = x + 35;
							FramePref.Box->position(FramePref.X, FramePref.Y);
							wind->redraw();
							//std::cout<<"FramePref.X="<<FramePref.X<<"\n";
							return 1;
						case FL_Up:
							//std::cout<<"Up Pressed\n";
							FramePref.Y -= HWBox+10;
							if(FramePref.Y<y + 35)
								FramePref.Y = (y+35) + (N-2)*10 + (N-2)*HWBox;
							FramePref.Box->position(FramePref.X, FramePref.Y);
							wind->redraw();
							return 1;
						case FL_Down:
							//std::cout<<"Down Pressed\n";
							FramePref.Y += HWBox+10;
							if(FramePref.Y > ((y+35) + (N-2)*10 + (N-2)*HWBox))
								FramePref.Y = y + 35;
							FramePref.Box->position(FramePref.X, FramePref.Y);
							wind->redraw();
							return 1;
						case 120:
							//std::cout<<"x pressed\n";
							TurnRight_(GetFramePos_());
							wind->redraw();
							CheckSolution();
							return 1;
						case 122:
							//std::cout<<"z pressed\n";
							TurnLeft_(GetFramePos_());
							wind->redraw();
							CheckSolution();
							return 1;
						case 115:
							pthread_create(&thrd,NULL,PrgsBar,(void*)NULL);
							if(FindSolution())
							{
								Calculated = true;
								//pthread_kill(thrd,SIGKILL);
								std::cout<<"Solution was find through "<<BackList.size()<<"steps in BackList\n";
								std::cout<<"Solution was find through "<<UsedList.size()<<"steps in UsedList\n";
								fl_message("Solution was find through %d steps", BackList.size());
								std::cout<<"iterations: "<<L<<std::endl;
								int _Tmp;
								for(unsigned i=0;i<BackList.size();i++)
								{
									/*std::cout<<std::endl;
									for(int j=0;j<N*N;j++)
										std::cout<<(int)BackList[i][j];*/
									//_Tmp = BackList[i]->Positions;
									for(auto k=0;k<N*N;k++)
									//_Tmp = (int)BackList[i]->Positions;
									{
										if(k % N == 0)std::cout<<"\n";
										_Tmp = (int)BackList[i]->Positions[k];
										std::cout<<std::setw(2)<<_Tmp<<" ";
									}
									std::cout<<std::endl;
								}
								Fl::add_timeout(0.25, TimerR, (void*)this);
							}
							else
							{
								fl_message("solution is not found");
								std::cout<<"solution is not found\n";
							}
							return 1;
					}
				}
		}
		return 0;
	}


	void CheckSolution()
	{
		for(int i=0;i<N*N/*N*(N-1)*/;i++)
		{
			if(BoxPref[i].Data != Solution[i])return;
		}
		for(int i=0;i<N*N;i++)BoxPref[i].Box->color(FL_BLUE);
		fl_message("you win");
		std::cout<<"You win\n";
		exit(0);
	}

	BoxesPreferences* GetFramePos_()
	{
		int Row;
		int Col;
		Row = FramePref.Box->y();
		Col = FramePref.Box->x();
		for(int i=0;i<N*N;i++)
		{
			//std::cout<<"enter to loop num:"<<i<<"\n";
			if((Col == BoxPref[i].X - 5)&&(Row == BoxPref[i].Y - 5))
			{
				//std::cout<<"return\n";
				return &BoxPref[i];
			}
		}
		//std::cout<<"come out\n";
		return NULL;
	}

	void TurnRight_(BoxesPreferences* BX)
	{
		int Buff;
		Buff = (BX)->Data;
		BX->Data = (BX+N)->Data;		
		(BX+N)->Data = (BX+N+1)->Data;		
		(BX+N+1)->Data = (BX+1)->Data;		
		(BX+1)->Data = Buff;		

		sprintf(BX->str, "%d",BX->Data);
		sprintf((BX+1)->str, "%d",(BX+1)->Data);
		sprintf((BX+N)->str, "%d",(BX+N)->Data);
		sprintf((BX+N+1)->str, "%d",(BX+N+1)->Data);
	}

	void TurnLeft_(BoxesPreferences* BX)
	{
		int Buff;
		Buff = BX->Data;
		BX->Data = (BX+1)->Data;		
		(BX+1)->Data = (BX+N+1)->Data ;		
		(BX+N+1)->Data = (BX+N)->Data;		
		(BX+N)->Data = Buff;		

		sprintf(BX->str, "%d",BX->Data);
		sprintf((BX+1)->str, "%d",(BX+1)->Data);
		sprintf((BX+N)->str, "%d",(BX+N)->Data);
		sprintf((BX+N+1)->str, "%d",(BX+N+1)->Data);
	}

	int GetManhattan(char *Value)
	{
		int Sum = 0;
		for(int i=0;i<N*N;i++)
			Sum += Calc(Value[i],i);
		return Sum;
	}

	int Calc(char Num, int Pos)
	{
		int Dif = 0;
		int x1,y1,x,y;
		x1 = Pos % N;
		y1 = Pos / N;

		x = ((int)Num - 1) % N;
		//x = (std::stoi(&Num) - 1) % N;
		y = ((int)Num - 1) / N;
		//y = (std::stoi(&Num) - 1) / N;

		Dif = std::abs( x - x1 ) + std::abs( y - y1);
		return Dif;
	}

	int FindSolution()
	{
		char *Comb = new char[N*N+1];
		Node = new _Nd;
		//_Nd *TmpNode = Node;
		std::memset(Node,0,sizeof(_Nd));
		for(int i=0;i<N*N;i++)
		{
			Comb[i] = BoxPref[i].Data;
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
		while(1)
		{
			for(char i=0; i<(N-1);i++)
			{
				for(char j=0; j<(N-1);j++)
				{
					for(char Left=0;Left<2;Left++)
					{
						//Creation
						/*Node->Child[x] = */Rotate(N*(int)i+(int)j, Node->Positions, Node, (bool)Left);
					}
				}
			}
			//auto It = std::min_element(Queue.begin(),Queue.end(),Comp1Func()); 
			auto It = Queue.begin();
			Node = *It;
			if(Node->hValue == 0)
			{

				//if(std::equal(Tmp, Tmp+(N*N), Solution))
				//{
					//std::cout<<"match with ideal\n";
					while(Node)
					{
						BackList.push_back(Node);
						if(Node->Parent) Node = Node->Parent;
						else break;
						//std::cout<<"ura\n";
					}
				Calculated = 1;
				return 1;
			}
					
			//delete Node;
			//auto pos = std::distance(Queue.begin(), It);
			//Node->hValue = 100;
			Queue.erase(It);//TODO
		}
		return 0;
	}

	int Rotate(char M, char* Output, _Nd *ParNode, char IsLeft)
	//int Rotate(char M, _Nd *ParNode, char IsLeft)
	//void PseudoLeft(int M, int* Output,std::vector<int*>& UL)
	{
		L++;

		_Nd* _Node = new _Nd;
		//_Nd *TmpNode = _Node;
		if(_Node == nullptr)std::cout<<"memory exhausted\n";
		//std::memset(Node,0,sizeof(_Nd));
		_Node->Parent = ParNode;
		//bool NotEqualFlag = false;
		char *Tmp = new char[N*N+1];
		if(Tmp == nullptr)std::cout<<"memory exhausted\n";
		std::copy(Output, Output+N*N, Tmp);
	//	std::copy(ParNode->Positions, ParNode->Positions+N*N, Tmp);
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
		_Node->Positions = Tmp;
		_Node->gValue = ParNode->gValue + 1;
		_Node->hValue = GetManhattan(_Node->Positions);
		_Node->fValue = _Node->gValue + _Node->hValue;
		/**/
		/**/
		//Insert combination to UsedList
		size_t sz;
		sz = UsedList.size();
		UsedList.insert(_Node->Positions);
		//Check existance in UsedList
		if(sz != UsedList.size() || _Node->hValue == 0)
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
};

void *PrgsBar(void *ptr)
{
	wind->begin();
	Fl_Double_Window* wd;
	wd = new Fl_Double_Window(wind->x()+wind->w()/2-110,wind->y()+wind->h()/2-50,220,100,"Wait Please.");
	wd->color(FL_LIGHT1);
	//wd->begin();
	Fl::check();
	Fl_Progress *progress = new Fl_Progress(10,50,200,30);
	progress->minimum(0);
	progress->maximum(1);
	progress->color(0x88888800);
	progress->selection_color(0x4444ff00);
	progress->labelcolor(FL_WHITE);
	wind->end();
	wd->show();
	int t=1;
	bool ToLeft = true;
	unsigned b=10;
	while(Calculated == 0)
	{
		progress->value(t/50.0);
		char percent[15];
		char Txt[] = "Processing...";
		//sprintf(percent, "%d%%", int((t/500.0)*100.0));
		strncpy(percent, Txt, b);
		for(unsigned i=0;i<14-b;i++)
		{
			percent[b+i] = ' ';
		}
		percent[14]='\0';
		b++;
		if(b>strlen(Txt))b=10;
		progress->label(percent);
		Fl::check();
		usleep(200000);
		if(ToLeft)t++;
		else t--;
		if(t>=50)ToLeft = false;
		if(t<=1)ToLeft = true;
	}
	wd->remove(progress);
	delete(progress);
	wind->remove(wd);
	delete(wd);
	wind->redraw();
}

void TimerR(void* UserData)
{
	Mybox* Mb = (Mybox*) UserData;
	static int ST = Mb->BackList.size()-1;
	int K = Mb->N;
	//if(Mb->BackList.size() > 0)
	if(ST >= 0)
	{
		for(auto x=0;x<K*K;x++)
		{
			char T = Mb->BackList[ST]->Positions[x];
			//std::cout<<(int)T<<std::endl;
			Mb->BoxPref[x].Data = (int)T;
			sprintf(Mb->BoxPref[x].str, "%d",Mb->BoxPref[x].Data);
		}
		//auto Iter =Mb->BackList[0];
		//Mb->BackList.erase(Mb->BackList[0]);
		wind->redraw();
		ST--;
		Fl::repeat_timeout(0.5, TimerR, UserData);
	}
	else
	{
		Mb->BackList.erase(Mb->BackList.begin(),Mb->BackList.end());
		Fl::remove_timeout(TimerR);
	}

}

int main()
{
	srand(time(NULL));
	Fl::scheme("gtk+");
	wind= new Fl_Double_Window(0,0,800,600,"User Interface");
	Mybox *Mb = new Mybox(150, 50, 500, 500, 4);
	wind->end();
        wind->show();
        return Fl::run();
}
