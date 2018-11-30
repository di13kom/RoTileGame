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

Mybox::~Mybox()
{
	delete[] Tiles;
}

Mybox::Mybox(Fl_Boxtype bt, int _x, int _y, int _w, int _h, int elemsCount) : Fl_Box(bt, _x, _y, _w, _h, ""),
//m_efsClass(elemsCount),
//m_bfsClass(elemsCount),
Solution(elemsCount * elemsCount)
{
	std::vector<char> Numb(elemsCount * elemsCount);
	TilesInRow = elemsCount;
	m_MainTableXpos = _x;
	m_MainTableYpos = _y;
	Tiles = new BoxesPreferences[TilesInRow*TilesInRow];
	std::iota(Numb.begin(), Numb.end(), 1);
	std::iota(Solution.begin(), Solution.end(), 1);
	//
	std::random_shuffle(Numb.begin(), Numb.end());
	for (int i = 0; i < TilesInRow*TilesInRow; i++)
	{
		Tiles[i].Data = Numb[i];
		sprintf(Tiles[i].str.data(), "%d", Tiles[i].Data);
	}
	int p = 0;
	Tile_Width_Height = (430 - (TilesInRow - 1) * InterTileDistance) / TilesInRow;
	int Frame_Width_Height = Tile_Width_Height * 2 + (FramePadding*2) + InterTileDistance;
	//std::cout<<"Tile_Width_Height:"<<Tile_Width_Height<<"\n";
	for (int i = 0; i < TilesInRow; i++)
	{
		for (int j = 0; j < TilesInRow; j++)
		{
			Tiles[p].X = (m_MainTableXpos + 40) + j * InterTileDistance + j * Tile_Width_Height;
			Tiles[p].Y = (m_MainTableYpos + 40) + i * InterTileDistance + i * Tile_Width_Height;
			p++;
		}
	}
	for (int i = 0; i < TilesInRow*TilesInRow; i++)
	{
		Tiles[i].Box = std::make_unique<Fl_Box>(FL_PLASTIC_UP_BOX, Tiles[i].X, Tiles[i].Y, Tile_Width_Height, Tile_Width_Height, Tiles[i].str.data());
		Tiles[i].Box->color(FL_GREEN);
		Tiles[i].Box->labelfont(FL_HELVETICA_BOLD);
		Tiles[i].Box->labelsize(40 * 3 / TilesInRow);
	}

	/*Frame.X = m_MainTableXpos + 145;
	  Frame.Y = m_MainTableYpos + 145;*/
	int FrmPosNum = (((TilesInRow - 1)*(TilesInRow - 1)) / 2) + 1;
	int yc = FrmPosNum / (TilesInRow - 1);
	int xc = (FrmPosNum % (TilesInRow - 1)) - 1;
	//std::cout<<"xc:"<<xc<<"\nyc:"<<yc<<"\n";
	Frame.X = (m_MainTableXpos + MainTablePadding) + xc * Tile_Width_Height + xc * InterTileDistance;
	Frame.Y = (m_MainTableYpos + MainTablePadding) + yc * Tile_Width_Height + yc * InterTileDistance;
	Frame.Box = std::make_unique<Fl_Box>(FL_PLASTIC_UP_FRAME, Frame.X, Frame.Y, Frame_Width_Height, Frame_Width_Height, "");
	Frame.Box->color(FL_RED);
#ifdef __unix__//To show iterations
	signal(SIGUSR1, Handler);
#endif // __unix__//To show 

}

void Mybox::draw()
{
	Fl_Box::draw();
	//wind->redraw();
	//std::cout<<"REdraw called\n";
	//fl_frame("AAAA", Frame.X, Frame.Y, 220, 220);
	//fl_color(FL_RED);
	//Fl_Box(FL_PLASTIC_UP_BOX, 10, 10, Tile_Width_Height, Tile_Width_Height,Tiles[15].str);
	//Frame->redraw();
	//Frame->resize(Frame.X + 110, Frame.Y, 20, 20);
	//redraw();
}

int Mybox::handle(int e)
{
	char *dataStr = new char[TilesInRow*TilesInRow];
	char* ptr = dataStr;
	switch (e)
	{
	case FL_SHORTCUT:
	{
		switch (Fl::event_key())
		{
		case FL_Left:
			//std::cout<<"Left Pressed\n";
			Frame.X -= Tile_Width_Height + InterTileDistance;
			if (Frame.X < m_MainTableXpos + MainTablePadding)
				Frame.X = (m_MainTableXpos + MainTablePadding) + (TilesInRow - 2) * InterTileDistance + (TilesInRow - 2)*Tile_Width_Height;
			Frame.Box->position(Frame.X, Frame.Y);
			wind->redraw();
			//std::cout<<"Frame.X="<<Frame.X<<"\n";
			return 1;
		case FL_Right:
			//std::cout<<"Right Pressed\n";
			Frame.X += Tile_Width_Height + InterTileDistance;
			if (Frame.X > ((m_MainTableXpos + MainTablePadding) + (TilesInRow - 2) * InterTileDistance + (TilesInRow - 2)*Tile_Width_Height))
				Frame.X = m_MainTableXpos + MainTablePadding;
			Frame.Box->position(Frame.X, Frame.Y);
			wind->redraw();
			//std::cout<<"Frame.X="<<Frame.X<<"\n";
			return 1;
		case FL_Up:
			//std::cout<<"Up Pressed\n";
			Frame.Y -= Tile_Width_Height + InterTileDistance;
			if (Frame.Y < m_MainTableYpos + MainTablePadding)
				Frame.Y = (m_MainTableYpos + MainTablePadding) + (TilesInRow - 2) * InterTileDistance + (TilesInRow - 2)*Tile_Width_Height;
			Frame.Box->position(Frame.X, Frame.Y);
			wind->redraw();
			return 1;
		case FL_Down:
			//std::cout<<"Down Pressed\n";
			Frame.Y += Tile_Width_Height + InterTileDistance;
			if (Frame.Y > ((m_MainTableYpos + MainTablePadding) + (TilesInRow - 2) * InterTileDistance + (TilesInRow - 2)*Tile_Width_Height))
				Frame.Y = m_MainTableYpos + MainTablePadding;
			Frame.Box->position(Frame.X, Frame.Y);
			wind->redraw();
			return 1;
		case 120:
			//std::cout<<"x pressed\n";
			TurnRight_(GetFrameLeftUpperPosition());
			wind->redraw();
			CheckSolution();
			return 1;
		case 122:
			//std::cout<<"z pressed\n";
			TurnLeft_(GetFrameLeftUpperPosition());
			wind->redraw();
			CheckSolution();
			return 1;
		case 'a':
			for (int i = 0; i < TilesInRow*TilesInRow; i++)
			{
				dataStr[i] = Tiles[i].Data;
			}

			m_fsClass = new BFS_Class(TilesInRow);
			m_fut = std::async(&BFS_Class::FindSolution, dynamic_cast<BFS_Class*>(m_fsClass), ptr);

			PrgsBar(nullptr);
			BackList = m_fut.get();
			if (BackList.size() > 0)
			{
				std::cout << "Solution was find through " << BackList.size() << "steps in BackList\n";
				fl_message("Solution was find through %d steps", BackList.size());
				//std::cout<<"iterations: "<<L<<std::endl;
				//for(unsigned i=0;i<BackList.size();i++)
				//{
				//	std::cout<<std::endl;
				//	for(int j=0;j<TilesInRow*TilesInRow;j++)
				//		std::cout<<(int)BackList[i][j];
				//}
			}
			else
			{
				fl_message("solution is not found");
				std::cout << "solution is not found\n";
			}
			return 1;
		case 's':
			for (int i = 0; i < TilesInRow*TilesInRow; i++)
			{
				dataStr[i] = Tiles[i].Data;
			}

			m_fsClass = new EFS_Class(TilesInRow);
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
					for (auto k = 0; k < TilesInRow*TilesInRow; k++)
					{
						if (k % TilesInRow == 0)std::cout << "\n";
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


void Mybox::CheckSolution()
{
	for (int i = 0; i < TilesInRow*TilesInRow/*N*(N-1)*/; i++)
	{
		if (Tiles[i].Data != Solution[i])return;
	}
	for (int i = 0; i < TilesInRow*TilesInRow; i++)Tiles[i].Box->color(FL_BLUE);
	fl_message("you win");
	std::cout << "You win\n";
	exit(0);
}

BoxesPreferences* Mybox::GetFrameLeftUpperPosition()
{
	int Row;
	int Col;
	Row = Frame.Box->y();
	Col = Frame.Box->x();
	for (int i = 0; i < TilesInRow*TilesInRow; i++)
	{
		//std::cout<<"enter to loop num:"<<i<<"\n";
		if ((Col == Tiles[i].X - FramePadding) && (Row == Tiles[i].Y - FramePadding))
		{
			//std::cout<<"return\n";
			return &Tiles[i];
		}
	}
	//std::cout<<"come out\n";
	return NULL;
}

void Mybox::TurnRight_(BoxesPreferences* BX)
{
	std::swap(BX->Data, (BX + 1)->Data);
	std::swap(BX->Data, (BX + TilesInRow + 1)->Data);
	std::swap(BX->Data, (BX + TilesInRow)->Data);

	sprintf(BX->str.data(), "%d", BX->Data);
	sprintf((BX + 1)->str.data(), "%d", (BX + 1)->Data);
	sprintf((BX + TilesInRow)->str.data(), "%d", (BX + TilesInRow)->Data);
	sprintf((BX + TilesInRow + 1)->str.data(), "%d", (BX + TilesInRow + 1)->Data);
}

void Mybox::TurnLeft_(BoxesPreferences* BX)
{
	std::swap((BX + 1)->Data, BX->Data);
	std::swap((BX + 1)->Data, (BX + TilesInRow)->Data);
	std::swap((BX + 1)->Data, (BX + TilesInRow + 1)->Data);

	sprintf(BX->str.data(), "%d", BX->Data);
	sprintf((BX + 1)->str.data(), "%d", (BX + 1)->Data);
	sprintf((BX + TilesInRow)->str.data(), "%d", (BX + TilesInRow)->Data);
	sprintf((BX + TilesInRow + 1)->str.data(), "%d", (BX + TilesInRow + 1)->Data);
}

void *Mybox::PrgsBar(void *ptr)
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
		std::this_thread::sleep_for(0.5s);
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


static void TimerR(void* UserData)
{
	Mybox* Mb = (Mybox*)UserData;
	static int ST = Mb->BackList.size() - 1;
	int K = Mb->TilesInRow;
	//if(Mb->BackList.size() > 0)
	if (ST >= 0)
	{
		for (auto x = 0; x < K*K; x++)
		{
			char T = Mb->BackList[ST][x];
			//std::cout<<(int)T<<std::endl;
			Mb->Tiles[x].Data = (int)T;
			sprintf(Mb->Tiles[x].str.data(), "%d", Mb->Tiles[x].Data);
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
	//srand(time(NULL));
	//Fl::scheme("gtk+");
	Fl_Double_Window windowX(0, 0, 800, 600, "Rotate N-Tiles Game");
	Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, 800, 25);              // Create menubar, items..
	menu->add("&File/&Open", "^o", nullptr);
	menu->add("&File/&Save", "^s", nullptr, 0, FL_MENU_DIVIDER);
	wind = &windowX;
	Mybox Mb(FL_PLASTIC_UP_FRAME, 150, 50, 500, 500, 6);
	windowX.end();
	windowX.show();
	return Fl::run();
}
