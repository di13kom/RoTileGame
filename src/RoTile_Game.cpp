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
}

Mybox::Mybox(Fl_Boxtype bt, int _x, int _y, int _w, int _h, int elemsCount) : Fl_Box(bt, _x, _y, _w, _h, ""),
	Solution(elemsCount * elemsCount)
{
	int xPos;
	int yPos;
	int yc;
	int xc;
	int Frame_Width_Height;
	int p = 0;
	std::vector<char> Numb(elemsCount * elemsCount);
	int FrmPosNum;
	color(FL_LIGHT1);

	TilesInRow = elemsCount;
	m_MainTableXpos = _x;
	m_MainTableYpos = _y;
	std::iota(Numb.begin(), Numb.end(), 1);
	std::iota(Solution.begin(), Solution.end(), 1);
	//
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(Numb.begin(), Numb.end(), g);
	//
	Tile_Width_Height = ((_w - s_MainTablePadding * 2) - (TilesInRow - 1) * s_InterTileDistance) / TilesInRow;
	Frame_Width_Height = Tile_Width_Height * 2 + (s_FramePadding * 2) + s_InterTileDistance;

	//std::cout<<"Tile_Width_Height:"<<Tile_Width_Height<<"\n";
	for (int i = 0; i < TilesInRow; i++)
	{
		for (int j = 0; j < TilesInRow; j++)
		{
			xPos = (m_MainTableXpos + s_MainTablePadding + s_FramePadding) + j * s_InterTileDistance + j * Tile_Width_Height;
			yPos = (m_MainTableYpos + s_MainTablePadding + s_FramePadding) + i * s_InterTileDistance + i * Tile_Width_Height;

			Tiles.emplace_back(BoxesPreferences(xPos, yPos, Tile_Width_Height, Tile_Width_Height, s_BasicFontSize * 3 / TilesInRow, Numb[p++]));
		}
	}

	/*Frame.X = m_MainTableXpos + 145;
	  Frame.Y = m_MainTableYpos + 145;*/
	FrmPosNum = (((TilesInRow - 1)*(TilesInRow - 1)) / 2) + 1;
	yc = FrmPosNum / (TilesInRow - 1);
	xc = (FrmPosNum % (TilesInRow - 1)) - 1;
	//std::cout<<"xc:"<<xc<<"\nyc:"<<yc<<"\n";


	xPos = (m_MainTableXpos + s_MainTablePadding) + xc * Tile_Width_Height + xc * s_InterTileDistance;
	yPos = (m_MainTableYpos + s_MainTablePadding) + yc * Tile_Width_Height + yc * s_InterTileDistance;


	Frame = std::make_unique<BoxesPreferences>(xPos, yPos, Frame_Width_Height, Frame_Width_Height);

#ifdef __unix__//To show iterations
	signal(SIGUSR1, Handler);
#endif // __unix__//To show 

	//for(auto& item : Tiles)
	//{
	//	item.SetData(item.GetData());
	//}
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
	int xTmp;
	int yTmp;
	switch (e)
	{
		case FL_SHORTCUT:
			{
				switch (Fl::event_key())
				{
					case FL_Left:
						//std::cout<<"Left Pressed\n";
						xTmp = Frame->GetX() - (Tile_Width_Height + s_InterTileDistance);
						if (xTmp < m_MainTableXpos + s_MainTablePadding)
							xTmp = (m_MainTableXpos + s_MainTablePadding) + (TilesInRow - 2) * s_InterTileDistance + (TilesInRow - 2)*Tile_Width_Height;
						Frame->SetX(xTmp);
						wind->redraw();

						return 1;
					case FL_Right:
						//std::cout<<"Right Pressed\n";
						xTmp = Frame->GetX() + (Tile_Width_Height + s_InterTileDistance);
						if (xTmp > ((m_MainTableXpos + s_MainTablePadding) + (TilesInRow - 2) * s_InterTileDistance + (TilesInRow - 2)*Tile_Width_Height))
							xTmp = m_MainTableXpos + s_MainTablePadding;
						Frame->SetX(xTmp);
						wind->redraw();

						return 1;
					case FL_Up:
						//std::cout<<"Up Pressed\n";
						yTmp = Frame->GetY() - (Tile_Width_Height + s_InterTileDistance);
						if (yTmp < m_MainTableYpos + s_MainTablePadding)
							yTmp = (m_MainTableYpos + s_MainTablePadding) + (TilesInRow - 2) * s_InterTileDistance + (TilesInRow - 2)*Tile_Width_Height;
						Frame->SetY(yTmp);
						wind->redraw();

						return 1;
					case FL_Down:
						//std::cout<<"Down Pressed\n";
						yTmp = Frame->GetY() + (Tile_Width_Height + s_InterTileDistance);
						if (yTmp > ((m_MainTableYpos + s_MainTablePadding) + (TilesInRow - 2) * s_InterTileDistance + (TilesInRow - 2)*Tile_Width_Height))
							yTmp = m_MainTableYpos + s_MainTablePadding;
						Frame->SetY(yTmp);
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
							dataStr[i] = Tiles[i].GetData();
							//dataStr[i] = Tiles[i].Data;
						}

						m_fsClass = new BFS_Class(TilesInRow);
						m_fut = std::async(&BFS_Class::FindSolution, dynamic_cast<BFS_Class*>(m_fsClass), ptr);

						PrgsBar(nullptr);
						BackList = m_fut.get();
						if (BackList.size() > 0)
						{
							std::cout << "Solution was found through " << BackList.size() << "steps in BackList\n";
							fl_message("Solution was found through %d steps", BackList.size());
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
							dataStr[i] = Tiles[i].GetData();
							//dataStr[i] = Tiles[i].Data;
						}

						m_fsClass = new EFS_Class(TilesInRow);
						m_fut = std::async(&EFS_Class::FindSolution, dynamic_cast<EFS_Class*>(m_fsClass), ptr);

						PrgsBar(nullptr);
						BackList = m_fut.get();
						if (BackList.size() > 0)
						{
							std::cout << "Solution was found through " << BackList.size() << "steps in BackList\n";
							//std::cout<<"Solution was find through "<<UsedList.size()<<"steps in UsedList\n";
							fl_message("Solution was found through %d steps", BackList.size());
							//std::cout<<"iterations: "<<L<<std::endl;
							int _Tmp;

							/*
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
							*/
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
		if (Tiles[i].GetData() != Solution[i])return;
	}
	for (int i = 0; i < TilesInRow*TilesInRow; i++)Tiles[i].SetColor(FL_BLUE);
	fl_message("you win");
	std::cout << "You win\n";
	exit(0);
}

int Mybox::GetFrameLeftUpperPosition()
{
	int retVal = 0;
	int Row = Frame->GetY();
	int Col = Frame->GetX();
	for (int i = 0; i < TilesInRow*TilesInRow; i++)
	{
		//std::cout<<"enter to loop num:"<<i<<"\n";
		if ((Col == Tiles[i].GetX() - s_FramePadding) && (Row == Tiles[i].GetY() - s_FramePadding))
		{
			retVal = i;
		}
	}
	//std::cout<<"come out\n";
	return retVal;
}

void Mybox::SetFramPositionByTileIndex(int ind)
{
	int x = Tiles[ind].GetX();
	int y = Tiles[ind].GetY();

	Frame->SetX(x - s_FramePadding);
	Frame->SetY(y - s_FramePadding);
}

void Mybox::TurnRight_(int bxInd)
{
	int Buff;
	Buff = Tiles[bxInd + 1].GetData();
	Tiles[bxInd + 1].SetData(Tiles[bxInd].GetData());
	Tiles[bxInd].SetData(Tiles[bxInd + TilesInRow].GetData());
	Tiles[bxInd + TilesInRow].SetData(Tiles[bxInd + TilesInRow + 1].GetData());
	Tiles[bxInd + TilesInRow + 1].SetData(Buff);
}

void Mybox::TurnLeft_(int bxInd)
{
	int Buff;
	Buff = Tiles[bxInd].GetData();
	Tiles[bxInd].SetData(Tiles[bxInd + 1].GetData());
	Tiles[bxInd + 1].SetData(Tiles[bxInd + TilesInRow + 1].GetData());

	Tiles[bxInd + TilesInRow + 1].SetData(Tiles[bxInd + TilesInRow].GetData());
	Tiles[bxInd + TilesInRow].SetData(Buff);
}

void *Mybox::PrgsBar(void *ptr)
{
	wind->begin();
	Fl_Double_Window wd(wind->x() + wind->w() / 2 - 110, wind->y() + wind->h() / 2 - 50, 220, 100, "Wait Please.");
	wd.border(1);
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
	static int currentIndex = Mb->BackList.size() - 1;
	int K = Mb->TilesInRow;
	static char* PrevArray = nullptr;

	//if(Mb->BackList.size() > 0)
	if (currentIndex >= 0)
	{
		char* currArray = Mb->BackList[currentIndex];

		char* currArrayElement = currArray;
		char* prevArraElement = PrevArray;
		int changedElemIndex;
		if(PrevArray != nullptr)
		{
			while(*currArrayElement == *prevArraElement)
			{
				currArrayElement++;
				prevArraElement++;
			}
			changedElemIndex = currArrayElement - currArray;

			Mb->SetFramPositionByTileIndex(changedElemIndex);
			//std::this_thread::sleep_for(0.2s);

			if(*currArrayElement == *(prevArraElement + 1))
			{
				Mb->TurnLeft_(changedElemIndex);
			}
			else
			{
				Mb->TurnRight_(changedElemIndex);
			}
			/*
			   auto vl = std::mismatch(std::begin(Mb->BackList[currentIndex]), std::end(Mb->BackList[currentIndex]), PrevArray);
			   if(*vl.first == *(vl+1).second)
			   {
			   Mb->TurnRight_(vl.first -  std::begin(Mb->BackList[currentIndex]));
			   }
			   else
			   {
			   Mb->TurnLeft_(vl.first -  std::begin(Mb->BackList[currentIndex]));
			   }
			   */




			/*
			   for (auto x = 0; x < K*K; x++)
			   {
			   char T = Mb->BackList[currentIndex][x];
			//std::cout<<(int)T<<std::endl;
			Mb->Tiles[x].SetData((int)T);
			}
			*/
			//auto Iter =Mb->BackList[0];
			//Mb->BackList.erase(Mb->BackList[0]);
			wind->redraw();
		}
		PrevArray = Mb->BackList[currentIndex];

		currentIndex--;
		Fl::repeat_timeout(0.5, TimerR, UserData);
	}
	else
	{
		Mb->BackList.erase(Mb->BackList.begin(), Mb->BackList.end());
		Fl::remove_timeout(TimerR);
	}

}

void callBack(Fl_Widget *wg, void *inp)
{
	std::cout << "test" << std::endl;
	int vl = atoi(((Fl_Int_Input*)inp)->value());
	std::cout << ((Fl_Int_Input*)inp)->value() << std::endl;
	wind->begin();
	Mybox Mb(FL_PLASTIC_UP_FRAME, 150, 50, 500, 500, vl);
	wind->end();
	Mb.show();
	//wind->redraw();
}

int main()
{
	Fl::scheme("gtk+");
	int vl = 0;
	do
	{
		auto r = fl_input("Enter value: from 3 - 10", "5");
		if (r != nullptr)
			vl = atoi(r);
	} while (vl < 3 || vl > 10);

	Fl_Double_Window windowX(0, 0, 800, 600, "Rotate N-Tiles Game");
	Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, 800, 25);              // Create menubar, items..
	menu->add("&File/&Open", "^o", nullptr);
	menu->add("&File/&Save", "^s", nullptr, 0, FL_MENU_DIVIDER);
	wind = &windowX;
	Mybox Mb(FL_DOWN_BOX, 150, 50, 500, 500, vl);
	windowX.end();
	windowX.show();

	//Fl_Double_Window window0(150, 150, 200, 70, "Enter value");
	//Fl_Int_Input vl(10, 10, 180, 30);
	//Fl_Button btn(140, 45, 50, 20, "Ok");
	//btn.callback(callBack, (void*)&vl);
	//window0.fullscreen_off();
	//window0.end();
	//window0.show();

	return Fl::run();
}
