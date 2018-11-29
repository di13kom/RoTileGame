#include "RoTile_Game.h"
using namespace std::chrono_literals;
int L;
Fl_Double_Window *wind;

static void TimerR(void*);

void Handler(int)
{
	std::cout << "SigSigv cathced" << std::endl;
	std::cout << "Itetations: " << L << std::endl;
}

class Mybox : public Fl_Box
{
	Fs* m_fsClass;
	//EFS_Class m_efsClass;
	//BFS_Class m_bfsClass;
	std::future<std::vector<char*>> m_fut;

	BoxesPreferences* BoxPref;
	BoxesPreferences FramePref;
	int x, y;
	int HWBox;
	int N;
	std::vector<char> Solution;
	std::vector<char*> BackList;
public:
	~Mybox()
	{
		delete[] BoxPref;
	}

	friend void TimerR(void*);
	Mybox(int _x, int _y, int _w, int _h, int elemsCount) : Fl_Box(_x, _y, _w, _h),
		//m_efsClass(elemsCount),
		//m_bfsClass(elemsCount),
		Solution(elemsCount * elemsCount)
	{
		std::vector<char> Numb(elemsCount * elemsCount);
		N = elemsCount;
		x = _x;
		y = _y;
		BoxPref = new BoxesPreferences[N*N];
		std::iota(Numb.begin(), Numb.end(), 1);
		std::iota(Solution.begin(), Solution.end(), 1);
		//
		std::random_shuffle(Numb.begin(), Numb.end());
		for (int i = 0; i < N*N; i++)
		{
			BoxPref[i].Data = Numb[i];
			sprintf(BoxPref[i].str.data(), "%d", BoxPref[i].Data);
		}
		int p = 0;
		HWBox = (430 - (N - 1) * 10) / N;
		int Frame_Width_Height = HWBox * 2 + 10 * 2;
		//std::cout<<"HWBox:"<<HWBox<<"\n";
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				BoxPref[p].X = (x + 40) + j * 10 + j * HWBox;
				BoxPref[p].Y = (y + 40) + i * 10 + i * HWBox;
				p++;
			}
		}
		for (int i = 0; i < N*N; i++)
		{
			BoxPref[i].Box = std::make_unique<Fl_Box>(FL_PLASTIC_UP_BOX, BoxPref[i].X, BoxPref[i].Y, HWBox, HWBox, BoxPref[i].str.data());
			BoxPref[i].Box->color(FL_GREEN);
			BoxPref[i].Box->labelfont(FL_HELVETICA_BOLD);
			BoxPref[i].Box->labelsize(40 * 3 / N);
		}

		/*FramePref.X = x + 145;
		  FramePref.Y = y + 145;*/
		int FrmPosNum = (((N - 1)*(N - 1)) / 2) + 1;
		int yc = FrmPosNum / (N - 1);
		int xc = (FrmPosNum % (N - 1)) - 1;
		//std::cout<<"xc:"<<xc<<"\nyc:"<<yc<<"\n";
		FramePref.X = (x + 35) + xc * HWBox + xc * 10;
		FramePref.Y = (y + 35) + yc * HWBox + yc * 10;
		FramePref.Box = std::make_unique<Fl_Box>(FL_PLASTIC_UP_FRAME, FramePref.X, FramePref.Y, Frame_Width_Height, Frame_Width_Height, "");
		FramePref.Box->color(FL_RED);
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
		char *dataStr = new char[N*N];
		char* ptr = dataStr;
		switch (e)
		{
		case FL_SHORTCUT:
		{
			switch (Fl::event_key())
			{
			case FL_Left:
				//std::cout<<"Left Pressed\n";
				FramePref.X -= HWBox + 10;
				if (FramePref.X < x + 35)
					FramePref.X = (x + 35) + (N - 2) * 10 + (N - 2)*HWBox;
				FramePref.Box->position(FramePref.X, FramePref.Y);
				wind->redraw();
				//std::cout<<"FramePref.X="<<FramePref.X<<"\n";
				return 1;
			case FL_Right:
				//std::cout<<"Right Pressed\n";
				FramePref.X += HWBox + 10;
				if (FramePref.X > ((x + 35) + (N - 2) * 10 + (N - 2)*HWBox))
					FramePref.X = x + 35;
				FramePref.Box->position(FramePref.X, FramePref.Y);
				wind->redraw();
				//std::cout<<"FramePref.X="<<FramePref.X<<"\n";
				return 1;
			case FL_Up:
				//std::cout<<"Up Pressed\n";
				FramePref.Y -= HWBox + 10;
				if (FramePref.Y < y + 35)
					FramePref.Y = (y + 35) + (N - 2) * 10 + (N - 2)*HWBox;
				FramePref.Box->position(FramePref.X, FramePref.Y);
				wind->redraw();
				return 1;
			case FL_Down:
				//std::cout<<"Down Pressed\n";
				FramePref.Y += HWBox + 10;
				if (FramePref.Y > ((y + 35) + (N - 2) * 10 + (N - 2)*HWBox))
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
			case 'a':
				for (int i = 0; i < N*N; i++)
				{
					dataStr[i] = BoxPref[i].Data;
				}

				m_fsClass = new BFS_Class(N);
				m_fut = std::async(&BFS_Class::FindSolution, dynamic_cast<BFS_Class*>(m_fsClass), ptr);

				PrgsBar(nullptr);
				BackList = m_fut.get();
				if (BackList.size() > 0)
				{
					std::cout << "Solution was find through " << BackList.size() << "steps in BackList\n";
					fl_message("Solution was find through %d steps", BackList.size());
					//std::cout<<"iterations: "<<L<<std::endl;
					for(unsigned i=0;i<BackList.size();i++)
					{
						std::cout<<std::endl;
						for(int j=0;j<N*N;j++)
							std::cout<<(int)BackList[i][j];
					}
				}
				else
				{
					fl_message("solution is not found");
					std::cout << "solution is not found\n";
				}
				return 1;
			case 's':
				for (int i = 0; i < N*N; i++)
				{
					dataStr[i] = BoxPref[i].Data;
				}

				m_fsClass = new EFS_Class(N);
				m_fut = std::async(&EFS_Class::FindSolution, dynamic_cast<EFS_Class*>(m_fsClass), ptr);

				PrgsBar(nullptr);
				BackList = m_fut.get();
				if (BackList.size() > 0)
				{
					std::cout << "Solution was find through " << BackList.size() << "steps in BackList\n";
					//std::cout<<"Solution was find through "<<UsedList.size()<<"steps in UsedList\n";
					fl_message("Solution was find through %d steps", BackList.size());
					//std::cout<<"iterations: "<<L<<std::endl;
					int _Tmp;
					for (unsigned i = 0; i < BackList.size(); i++)
					{
						for (auto k = 0; k < N*N; k++)
						{
							if (k % N == 0)std::cout << "\n";
							_Tmp = (int)BackList[i][k];
							std::cout << std::setw(2) << _Tmp << " ";
						}
						std::cout << std::endl;
					}
					Fl::add_timeout(0.25, TimerR, (void*)this);
				}
				else
				{
					fl_message("solution is not found");
					std::cout << "solution is not found\n";
				}
				return 1;
			}
		}
		}
		return 0;
	}


	void CheckSolution()
	{
		for (int i = 0; i < N*N/*N*(N-1)*/; i++)
		{
			if (BoxPref[i].Data != Solution[i])return;
		}
		for (int i = 0; i < N*N; i++)BoxPref[i].Box->color(FL_BLUE);
		fl_message("you win");
		std::cout << "You win\n";
		exit(0);
	}

	BoxesPreferences* GetFramePos_()
	{
		int Row;
		int Col;
		Row = FramePref.Box->y();
		Col = FramePref.Box->x();
		for (int i = 0; i < N*N; i++)
		{
			//std::cout<<"enter to loop num:"<<i<<"\n";
			if ((Col == BoxPref[i].X - 5) && (Row == BoxPref[i].Y - 5))
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
		std::swap(BX->Data, (BX + 1)->Data);
		std::swap(BX->Data, (BX + N + 1)->Data);
		std::swap(BX->Data, (BX + N)->Data);

		sprintf(BX->str.data(), "%d", BX->Data);
		sprintf((BX + 1)->str.data(), "%d", (BX + 1)->Data);
		sprintf((BX + N)->str.data(), "%d", (BX + N)->Data);
		sprintf((BX + N + 1)->str.data(), "%d", (BX + N + 1)->Data);
	}

	void TurnLeft_(BoxesPreferences* BX)
	{
		std::swap((BX + 1)->Data, BX->Data);
		std::swap((BX + 1)->Data, (BX + N)->Data);
		std::swap((BX + 1)->Data, (BX + N + 1)->Data);

		sprintf(BX->str.data(), "%d", BX->Data);
		sprintf((BX + 1)->str.data(), "%d", (BX + 1)->Data);
		sprintf((BX + N)->str.data(), "%d", (BX + N)->Data);
		sprintf((BX + N + 1)->str.data(), "%d", (BX + N + 1)->Data);
	}

	void *PrgsBar(void *ptr)
	{
		wind->begin();
		Fl_Double_Window wd(wind->x() + wind->w() / 2 - 110, wind->y() + wind->h() / 2 - 50, 220, 100, "Wait Please.");
		wd.color(FL_LIGHT1);
		//wd->begin();
		Fl::check();
		Fl_Progress progress(10, 50, 200, 30);
		progress.minimum(0);
		progress.maximum(1);
		progress.color(0x88888800);
		progress.selection_color(0x4444ff00);
		progress.labelcolor(FL_WHITE);
		wind->end();
		wd.show();
		int t = 1;
		bool ToLeft = true;
		unsigned b = 10;
		std::future_status status;
		do
		{
			status = m_fut.wait_for(std::chrono::milliseconds(100));
			progress.value(t / 50.0);
			//std::array<char,15> percent{"Processing..."};
			char percent[15];
			char Txt[] = "Processing...";
			//sprintf(percent, "%d%%", int((t/500.0)*100.0));
			strncpy(percent, Txt, b);
			for (unsigned i = 0; i < 14 - b; i++)
			{
				percent[b + i] = ' ';
			}
			percent[14] = '\0';
			b++;
			if (b > strlen(Txt))b = 10;
			progress.label(percent);
			Fl::check();
			std::this_thread::sleep_for(2s);
			//usleep(200000);
			if (ToLeft)t++;
			else t--;
			if (t >= 50)ToLeft = false;
			if (t <= 1)ToLeft = true;
		} while (status != std::future_status::ready);
		wd.remove(&progress);
		//delete progress;
		wind->remove(&wd);
		//delete wd;
		wind->redraw();

		return nullptr;
	}
};


static void TimerR(void* UserData)
{
	Mybox* Mb = (Mybox*)UserData;
	static int ST = Mb->BackList.size() - 1;
	int K = Mb->N;
	//if(Mb->BackList.size() > 0)
	if (ST >= 0)
	{
		for (auto x = 0; x < K*K; x++)
		{
			char T = Mb->BackList[ST][x];
			//std::cout<<(int)T<<std::endl;
			Mb->BoxPref[x].Data = (int)T;
			sprintf(Mb->BoxPref[x].str.data(), "%d", Mb->BoxPref[x].Data);
		}
		//auto Iter =Mb->BackList[0];
		//Mb->BackList.erase(Mb->BackList[0]);
		wind->redraw();
		ST--;
		Fl::repeat_timeout(0.5, TimerR, UserData);
	}
	else
	{
		Mb->BackList.erase(Mb->BackList.begin(), Mb->BackList.end());
		Fl::remove_timeout(TimerR);
	}

}

int main()
{
	srand(time(NULL));
	Fl::scheme("gtk+");
	Fl_Double_Window windowX(0, 0, 800, 600, "User Interface");
	Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, 800, 25);              // Create menubar, items..
	menu->add("&File/&Open", "^o", nullptr);
	menu->add("&File/&Save", "^s", nullptr, 0, FL_MENU_DIVIDER);
	wind = &windowX;
	Mybox Mb(150, 50, 500, 500, 3);
	windowX.end();
	windowX.show();
	return Fl::run();
}
