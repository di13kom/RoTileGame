#include "RoTile_Game.h"
using namespace std::chrono_literals;
//
int L;
Fl_Double_Window *wind;

//Define static
int Tile::Width_Height = 0;
int Tile::FontSize = 0;

int Frame::Width_Height = 0;

static void TimerR(void*);

void Handler(int)
{
	std::cout << "SigSigv cathced" << std::endl;
	std::cout << "Itetations: " << L << std::endl;
}

Mybox::~Mybox()
{
}

Mybox::Mybox(Fl_Boxtype bt, int _x, int _y, int _wh) : Fl_Box(bt, _x, _y, _wh, _wh, "")
{

	m_MainTableXpos = _x;
	m_MainTableYpos = _y;
	m_MainTableWidthHeight = _wh;
	GameState = GameStateEnum::Empty;
#ifdef __unix__//To show iterations
	signal(SIGUSR1, Handler);
#endif // __unix__//To show 
}

void Mybox::draw()
{
	Fl_Box::draw();
	//std::cout<<"Redraw called\n";
	if (GameState >= GameStateEnum::Initialization)
	{
		//std::cout<<"Redraw called with Frame\n";

		fl_font(FL_HELVETICA_BOLD, Tile::FontSize);
		for (auto& item : Tiles)
		{
			fl_draw_box(FL_PLASTIC_UP_BOX, item.GetX(), item.GetY()
				, Tile::Width_Height, Tile::Width_Height, item.GetColor());
			fl_color(FL_BLACK);
			fl_draw(item.GetRawData(), item.GetX(), item.GetY(), Tile::Width_Height \
				, Tile::Width_Height, FL_ALIGN_CENTER, nullptr, 2);
		}

		if (GameState != GameStateEnum::FrameDragging)
		{
			fl_draw_box(FL_PLASTIC_UP_FRAME
				, m_Frame.GetX()
				, m_Frame.GetY()
				, Frame::Width_Height
				, Frame::Width_Height
				, m_Frame.GetColor());
		}
		else
		{
			fl_draw_box(FL_PLASTIC_UP_FRAME
				, m_VisualDragFrame.GetX()
				, m_VisualDragFrame.GetY()
				, Frame::Width_Height
				, Frame::Width_Height
				, FL_CYAN);
		}
		if (GameState == GameStateEnum::TileDragging)
		{
			fl_draw_box(FL_PLASTIC_UP_BOX
				, m_VisualDragTile.VisualTile.GetX()
				, m_VisualDragTile.VisualTile.GetY()
				, Tile::Width_Height
				, Tile::Width_Height
				, FL_CYAN);
			fl_color(FL_BLACK);
			fl_draw(m_VisualDragTile.VisualTile.GetRawData()
				, m_VisualDragTile.VisualTile.GetX()
				, m_VisualDragTile.VisualTile.GetY()
				, Tile::Width_Height
				, Tile::Width_Height
				, FL_ALIGN_CENTER
				, nullptr, 2);
		}

	}
}

void Mybox::SetTilesValue(std::vector<int> inValues)
{
	int xPos;
	int yPos;
	int p = 0;
	bool isBorderTile;
	//int FrmPosNum;
	color(FL_LIGHT1);

	GameState = GameStateEnum::Initialization;

	TilesInRow = std::sqrt(inValues.size());
	Tile::FontSize = s_BasicFontValue / TilesInRow;

	//set Solution
	std::copy(std::begin(inValues), std::end(inValues), std::back_inserter(Solution));
	std::sort(std::begin(Solution), std::end(Solution));
	//
	Tile::Width_Height = ((m_MainTableWidthHeight - s_MainTablePadding * 2)\
		- (TilesInRow - 1) * s_InterTileDistance) / TilesInRow;
	Frame::Width_Height = Tile::Width_Height * 2 + (s_FramePadding * 2) + s_InterTileDistance;

	//std::cout<<"Tile::Width_Height:"<<Tile::Width_Height<<"\n";
	for (int i = 0; i < TilesInRow; i++)
	{
		for (int j = 0; j < TilesInRow; j++)
		{
			if (i == TilesInRow - 1 || j == TilesInRow - 1)
				isBorderTile = true;
			else
				isBorderTile = false;

			xPos = (m_MainTableXpos + s_MainTablePadding + s_FramePadding)\
				+ j * s_InterTileDistance + j * Tile::Width_Height;
			yPos = (m_MainTableYpos + s_MainTablePadding + s_FramePadding)\
				+ i * s_InterTileDistance + i * Tile::Width_Height;

			Tiles.emplace_back(Tile(xPos
				, yPos
				, Tile::Width_Height
				, Tile::Width_Height
				, isBorderTile
				, FL_GREEN
				, Tile::FontSize
				, static_cast<char>(inValues[p++])));
		}
	}

	SetFramePositionByTileIndex(0);

	m_Frame.SetColor(FL_RED);
	redraw();
}

int Mybox::handle(int e)
{
	int xTmp;
	int yTmp;
	std::unique_ptr<char[]> dataStr;
	std::unique_ptr<Fs> fsClass;
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> end;
	std::chrono::duration<double> diff;
	//std::cout << "GameState: " << static_cast<int>(GameState) << std::endl;
	switch (e)
	{
	case FL_PUSH:
	{
		if (GameState != GameStateEnum::FindSolving)
		{
			if (Fl::event_button() == FL_LEFT_MOUSE)
			{
				if (Fl::event_inside(m_Frame.GetX()
					, m_Frame.GetY()
					, Frame::Width_Height
					, Frame::Width_Height))
				{
					GameState = GameStateEnum::FrameDragging;
					m_VisualDragFrame = m_Frame;
				}
			}
			else if (Fl::event_button() == FL_RIGHT_MOUSE)
			{
				auto itr = std::find_if(Tiles.begin()
					, Tiles.end()
					, [&](const Tile& box)
				{
					return Fl::event_inside(box.GetX()
						, box.GetY()
						, Tile::Width_Height
						, Tile::Width_Height)
						&&
						Fl::event_inside(m_Frame.GetX()
							, m_Frame.GetY()
							, Frame::Width_Height
							, Frame::Width_Height);
				});
				if (itr != Tiles.end())
				{
					bool isLeftTile;
					bool isUpperTile;

					std::vector<Tile>::iterator ULpairItr = itr;

					if (itr->GetX() > m_Frame.GetX() + Frame::Width_Height / 2)
					{
						//std::cout<<"right tile\n";
						isLeftTile = false;
					}
					else
					{
						//std::cout<<"left tile\n";
						isLeftTile = true;
					}
					if (itr->GetY() < m_Frame.GetY() + Frame::Width_Height / 2)
					{
						isUpperTile = true;
						//std::cout<<"upper tile\n";
					}
					else
					{
						isUpperTile = false;
						//std::cout<<"lower tile\n";
					}

					if (isLeftTile == true)
					{
						auto LRpairItr = std::next(itr);
						LRpairItr->SetColor(FL_BLUE);

					}
					else
					{
						auto LRpairItr = std::prev(itr);
						LRpairItr->SetColor(FL_BLUE);

					}
					if (isUpperTile == true)
					{
						std::advance(ULpairItr, TilesInRow);
						ULpairItr->SetColor(FL_BLUE);
					}
					else
					{
						std::advance(ULpairItr, -TilesInRow);
						ULpairItr->SetColor(FL_BLUE);
					}
					redraw();

					//std::cout<<"Inside"<<std::endl;

					m_VisualDragTile.VisualTile = *itr;
					m_VisualDragTile.TileIterator = itr;
					m_VisualDragTile.IsUpperTile = isUpperTile;
					m_VisualDragTile.IsLeftTile = isLeftTile;

					GameState = GameStateEnum::TileDragging;
				}
				//else
				//	std::cout<<"Outside"<<std::endl;
			}
		}
		return 1;
	}
	case FL_DRAG:
	{
		switch (GameState)
		{
		case GameStateEnum::FrameDragging:
		{
			m_VisualDragFrame.SetX(Fl::event_x() - Frame::Width_Height / 2);
			m_VisualDragFrame.SetY(Fl::event_y() - Frame::Width_Height / 2);
			redraw();
			break;
		}
		case GameStateEnum::TileDragging:
		{
			m_VisualDragTile.VisualTile.SetX(Fl::event_x() - Tile::Width_Height / 2);
			m_VisualDragTile.VisualTile.SetY(Fl::event_y() - Tile::Width_Height / 2);
			redraw();
			break;
		}
		default:
			break;
		}
		return 1;
	}
	case FL_RELEASE:
	{
		switch (GameState)
		{
		case GameStateEnum::FrameDragging:
		{
			auto itr = std::find_if(Tiles.begin(), Tiles.end(), [&](const Tile& box)
			{
				return Fl::event_inside(box.GetX()
					, box.GetY()
					, Frame::Width_Height - s_FramePadding * 2
					, Frame::Width_Height - s_FramePadding * 2)
					&&
					box.GetIsBorderTile() == false;
				;
			});
			if (itr != Tiles.end())
			{
				//std::cout<<"Frame found\n";
				m_Frame.SetX(itr->GetX() - s_FramePadding);
				m_Frame.SetY(itr->GetY() - s_FramePadding);
			}
			GameState = GameStateEnum::Normal;
			redraw();
			break;
		}
		case GameStateEnum::TileDragging:
		{
			//std::cout<<"X: "<<m_VisualDragTile.VisualTile.GetX()<<"Y: "<<m_VisualDragTile.VisualTile.GetY()<<std::endl;
			std::vector<Tile>::iterator clockWiseTileIter = m_VisualDragTile.TileIterator;
			std::vector<Tile>::iterator counterClockWiseTileIter = m_VisualDragTile.TileIterator;

			if (m_VisualDragTile.IsLeftTile == true
				&& m_VisualDragTile.IsUpperTile == true)
			{

				std::advance(clockWiseTileIter, 1);
				std::advance(counterClockWiseTileIter, TilesInRow);
				//std::cout<<"LU"<<std::endl;
			}
			else if (m_VisualDragTile.IsLeftTile == true
				&& m_VisualDragTile.IsUpperTile == false)
			{
				std::advance(clockWiseTileIter, -TilesInRow);
				std::advance(counterClockWiseTileIter, 1);
				//std::cout<<"LD"<<std::endl;
			}
			else if (m_VisualDragTile.IsLeftTile == false
				&& m_VisualDragTile.IsUpperTile == false)
			{
				std::advance(clockWiseTileIter, -1);
				std::advance(counterClockWiseTileIter, TilesInRow*-1);
				//std::cout<<"RD"<<std::endl;
			}
			else if (m_VisualDragTile.IsLeftTile == false
				&& m_VisualDragTile.IsUpperTile == true)
			{
				std::advance(clockWiseTileIter, TilesInRow);
				std::advance(counterClockWiseTileIter, -1);
				//std::cout<<"RU"<<std::endl;
			}
			else
			{
				clockWiseTileIter = Tiles.end();
				counterClockWiseTileIter = Tiles.end();
			}


			if (clockWiseTileIter != Tiles.end()
				&& counterClockWiseTileIter != Tiles.end())
			{
				if (Fl::event_inside(clockWiseTileIter->GetX()
					, clockWiseTileIter->GetY()
					, Tile::Width_Height
					, Tile::Width_Height))
				{
					TurnRight(GetFrameLeftUpperPosition());
					//std::cout<<"clockWise"<<std::endl;
				}
				else if (Fl::event_inside(counterClockWiseTileIter->GetX()
					, counterClockWiseTileIter->GetY()
					, Tile::Width_Height
					, Tile::Width_Height))
				{
					TurnLeft(GetFrameLeftUpperPosition());
					//std::cout<<"coutnerClockWise"<<std::endl;
				}
			}

			for (auto& vl : Tiles)
			{
				vl.SetColor(FL_GREEN);
			}

			GameState = GameStateEnum::Normal;
			redraw();
			break;
		}
		default:
			break;
		}
		return 1;
	}
	case FL_SHORTCUT:
	{
		if (GameState == GameStateEnum::Normal || GameState == GameStateEnum::Initialization)
		{
			switch (Fl::event_key())
			{
			case FL_Left:
				xTmp = m_Frame.GetX() - (Tile::Width_Height + s_InterTileDistance);
				if (xTmp < m_MainTableXpos + s_MainTablePadding)
					xTmp = (m_MainTableXpos + s_MainTablePadding) + (TilesInRow - 2)\
					* s_InterTileDistance + (TilesInRow - 2)*Tile::Width_Height;
				m_Frame.SetX(xTmp);
				redraw();

				return 1;
			case FL_Right:
				xTmp = m_Frame.GetX() + (Tile::Width_Height + s_InterTileDistance);
				if (xTmp > ((m_MainTableXpos + s_MainTablePadding) + (TilesInRow - 2)\
					* s_InterTileDistance + (TilesInRow - 2)*Tile::Width_Height))
					xTmp = m_MainTableXpos + s_MainTablePadding;
				m_Frame.SetX(xTmp);
				redraw();

				return 1;
			case FL_Up:
				yTmp = m_Frame.GetY() - (Tile::Width_Height + s_InterTileDistance);
				if (yTmp < m_MainTableYpos + s_MainTablePadding)
					yTmp = (m_MainTableYpos + s_MainTablePadding) + (TilesInRow - 2)\
					* s_InterTileDistance + (TilesInRow - 2)*Tile::Width_Height;
				m_Frame.SetY(yTmp);
				redraw();

				return 1;
			case FL_Down:
				yTmp = m_Frame.GetY() + (Tile::Width_Height + s_InterTileDistance);
				if (yTmp > ((m_MainTableYpos + s_MainTablePadding) + (TilesInRow - 2)\
					* s_InterTileDistance + (TilesInRow - 2)*Tile::Width_Height))
					yTmp = m_MainTableYpos + s_MainTablePadding;
				m_Frame.SetY(yTmp);
				redraw();

				return 1;
			case 'x':
				TurnRight(GetFrameLeftUpperPosition());
				redraw();
				CheckSolution();
				return 1;
			case 'z':
				TurnLeft(GetFrameLeftUpperPosition());
				redraw();
				CheckSolution();
				return 1;
				/* Too much memory consume
			case 'a':

				dataStr = std::make_unique<char[]>(TilesInRow*TilesInRow);
				std::transform(Tiles.begin()
						, Tiles.end()
						, dataStr.get()
						, [](const Tile& box)
						{
							return box.GetData();
						});

				start = std::chrono::system_clock::now();

				fsClass = std::make_unique<BFS_Class>(TilesInRow);
				m_fut = std::async(&BFS_Class::FindSolution
							, dynamic_cast<BFS_Class*>(fsClass.get())
							, dataStr.get());

				PrgsBar(nullptr);
				BackList = m_fut.get();

				end = std::chrono::system_clock::now();
				diff = end - start;
				std::cout << "Time elapsed: " << diff.count()<<std::endl;

				if (BackList.size() > 0)
				{
					std::cout << "Solution was found through " \
						<< BackList.size() - 1 << " steps in BackList\n";
					fl_message("Solution was found through %d steps", BackList.size() - 1);
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
				*/
			case 's':
				dataStr = std::make_unique<char[]>(TilesInRow*TilesInRow + 1);
				dataStr[TilesInRow*TilesInRow] = '\0';
				std::transform(Tiles.begin()
					, Tiles.end()
					, dataStr.get()
					, [](const Tile& box)
				{
					return box.GetData();
				});

				start = std::chrono::system_clock::now();

				fsClass = std::make_unique<EFS_Class>(TilesInRow);
				m_fut = std::async(&EFS_Class::FindSolution
					, dynamic_cast<EFS_Class*>(fsClass.get())
					, std::move(dataStr));

				GameState = GameStateEnum::FindSolving;

				PrgsBar(fsClass.get());
				BackList = m_fut.get();

				end = std::chrono::system_clock::now();
				diff = end - start;
				std::cout << "Time elapsed: " << diff.count() << std::endl;
				if (BackList.size() > 0)
				{
					fl_message("Solution was found through %lu steps", BackList.size() - 1);
#ifdef DEBUGLOG
					int _Tmp;

					for (int i = BackList.size() - 1; i >= 0; i--)
					{
						for (auto k = 0; k < TilesInRow*TilesInRow; k++)
						{
							if (k % TilesInRow == 0)std::cout << "\n";
							_Tmp = (int)BackList[i][k];
							std::cout << std::setw(2) << _Tmp << " ";
						}
						std::cout << std::endl;
					}
#endif
					std::cout << "Solution was found through "\
						<< BackList.size() - 1 << " steps in BackList\n";
					std::cout << "iterations: " << fsClass->GetIteration() << std::endl;
					std::cout << "UsedList: " << fsClass->GetUsedListCount() << std::endl;

					Fl::add_timeout(0.25, TimerR, (void*)this);
				}
				else
				{
					fl_message("solution is not found");
					std::cout << "solution is not found\n";
					GameState = GameStateEnum::Solved;
				}
				return 1;
			}
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
	//for (int i = 0; i < TilesInRow*TilesInRow; i++)Tiles[i].SetColor(FL_BLUE);
	fl_message("you win");
	std::cout << "You win\n";
	exit(0);
}

int Mybox::GetFrameLeftUpperPosition()
{
	int retVal = 0;
	int Row = m_Frame.GetY();
	int Col = m_Frame.GetX();
	for (int i = 0; i < TilesInRow*TilesInRow; i++)
	{
		//std::cout<<"enter to loop num:"<<i<<"\n";
		if ((Col == Tiles[i].GetX() - s_FramePadding) && (Row == Tiles[i].GetY() - s_FramePadding))
		{
			retVal = i;
			break;
		}
	}
	return retVal;
}

void Mybox::SetFramePositionByTileIndex(int ind)
{
	int x = Tiles[ind].GetX();
	int y = Tiles[ind].GetY();

	m_Frame.SetX(x - s_FramePadding);
	m_Frame.SetY(y - s_FramePadding);
}

void Mybox::TurnRight(int bxInd)
{
	int Buff;
	Buff = Tiles[bxInd + 1].GetData();
	Tiles[bxInd + 1].SetData(Tiles[bxInd].GetData());
	Tiles[bxInd].SetData(Tiles[bxInd + TilesInRow].GetData());
	Tiles[bxInd + TilesInRow].SetData(Tiles[bxInd + TilesInRow + 1].GetData());
	Tiles[bxInd + TilesInRow + 1].SetData(Buff);
}

void Mybox::TurnLeft(int bxInd)
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
	Fl_Double_Window wd(wind->x() + wind->w() / 2 - 110, wind->y() + wind->h() / 2 - 50, 220, 120, "Wait Please.");
	wd.border(1);
	wd.color(FL_LIGHT1);
	Fl::check();
	Fl_Progress progress(10, 50, 200, 30);
	progress.minimum(0);
	progress.maximum(1);
	progress.color(0x88888800);
	progress.selection_color(0x4444ff00);
	progress.labelcolor(FL_WHITE);
	Fl_Return_Button btnX(130, 90, 80, 25, "Cancel");

	btnX.callback(&Mybox::CancelCallback, (void*)ptr);

	wind->end();
	wd.show();
	float progressLineFillValue = 1;
	bool ToLeft = true;
	unsigned counter = 10;
	std::future_status status;
	do
	{
		status = m_fut.wait_for(std::chrono::milliseconds(100));
		progress.value(progressLineFillValue / 50.0);
		//std::array<char,15> percent{"Processing..."};
		char percent[15];
		char Txt[] = "Processing...";
		//sprintf(percent, "%d%%", int((progressLineFillValue/500.0)*100.0));
		strncpy(percent, Txt, counter);
		for (unsigned i = 0; i < 14 - counter; i++)
		{
			percent[counter + i] = ' ';
		}
		percent[14] = '\0';
		counter++;
		if (counter > strlen(Txt))counter = 10;
		progress.label(percent);
		Fl::check();
		std::this_thread::sleep_for(0.9s);
		//usleep(200000);
		if (ToLeft)progressLineFillValue++;
		else progressLineFillValue--;
		if (progressLineFillValue >= 50)ToLeft = false;
		if (progressLineFillValue <= 1)ToLeft = true;
	} while (status != std::future_status::ready);
	wd.remove(&progress);
	//delete progress;
	wind->remove(&wd);
	wind->redraw();

	return nullptr;
}


void Mybox::CancelCallback(Fl_Widget *wg, void* v)
{
	Fs* cls = static_cast<Fs*>(v);
	cls->CancelTask();
}

static void TimerR(void* UserData)
{
	Mybox* Mb = (Mybox*)UserData;
	static int currentIndex = Mb->BackList.size() - 1;
	//int K = Mb->TilesInRow;
	//static char* PrevArray = nullptr;

	if (currentIndex >= 1)
	{
		auto& previousData = Mb->BackList[currentIndex];
		auto& currentData = Mb->BackList[currentIndex - 1];

		auto mismatch = std::mismatch(std::begin(previousData), std::end(previousData), std::begin(currentData), std::end(currentData));
		auto distance = std::distance(std::begin(currentData), mismatch.second);

		Mb->SetFramePositionByTileIndex(distance);
		if (mismatch.first[0] == mismatch.second[1])
		{
			Mb->TurnRight(distance);
		}
		else
		{
			Mb->TurnLeft(distance);
		}
		wind->redraw();

		/*Old*/

		//std::vector<char>& previousData = Mb->BackList[currentIndex];

		//char currArrayElement = previousData;
		//char prevArraElement = currentData;
		//int changedElemIndex;
		//if (currentData != nullptr)
		//{
		//	std::mismatch(std::begin(previousData), std::end(previousData), currentData);

		//	while (*currArrayElement == *prevArraElement)
		//	{
		//		currArrayElement++;
		//		prevArraElement++;
		//	}
		//	changedElemIndex = currArrayElement - previousData;

		//	Mb->SetFramePositionByTileIndex(changedElemIndex);
		//	//std::this_thread::sleep_for(0.2s);

		//	if (*currArrayElement == *(prevArraElement + 1))
		//	{
		//		Mb->TurnLeft(changedElemIndex);
		//	}
		//	else
		//	{
		//		Mb->TurnRight(changedElemIndex);
		//	}
		//	wind->redraw();
		//}
		//currentData = Mb->BackList[currentIndex];

		currentIndex--;
		Fl::repeat_timeout(0.9, TimerR, UserData);
	}
	else
	{
		Mb->BackList.erase(Mb->BackList.begin(), Mb->BackList.end());
		Fl::remove_timeout(TimerR);

		Mb->GameState = GameStateEnum::Solved;
	}

}

std::vector<int> CreateRandomField(int elemsCount)
{
	std::vector<int> retVal(elemsCount * elemsCount);

	std::iota(retVal.begin(), retVal.end(), 1);
	//std::copy(retVal.begin(), retVal.end(), std::back_inserter(Solution));
	//
	//std::sort(std::begin(retVal), std::end(retVal), std::greater<char>());

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(retVal.begin(), retVal.end(), g);

	return retVal;
}

void callBack(Fl_Widget *wg, void *inp)
{
	auto* inpt = dynamic_cast<Fl_Int_Input*>(wind->child(1));
	auto* btn = dynamic_cast<Fl_Return_Button*>(wind->child(2));
	auto* mb = dynamic_cast<Mybox*>(wind->child(3));

	int vl = atoi(inpt->value());
	if (vl >= 3 && vl <= 9)
	{
		mb->SetTilesValue(CreateRandomField(vl));

		inpt->deactivate();
		//inpt->clear_visible_focus();
		//inpt->clear_active();
		//inpt->hide();

		btn->deactivate();
		//btn->clear_active();

		//mb->draw_focus();
		mb->active();
		//mb->take_focus();
		//mb->set_visible_focus();
		mb->show();

		//wind->flush();
		//wg->parent()->hide();
	}
	else
	{
		inpt->value(NULL);
	}
}

ParseFileValue OpenFileAndLaunch(char * fileName)
{
	std::fstream inFile;
	std::string linecontent;
	std::regex reg("(\\d{1,2})(?:,|$)");
	std::smatch reg_match;
	std::vector<int> targetVector;
	int linesCount = 0;
	int prevRowsCount = 0;
	int curRowsCount = 0;
	int inputValuesCount;
	std::ostringstream errorStringStream;
	ParseFileValue retVal;

	inFile.open(fileName, std::ifstream::in);
	if (!inFile.is_open())
	{
		errorStringStream << "error on open file: " << fileName;
		retVal.ErrorMsg = errorStringStream.str();
	}
	else
	{
		while (std::getline(inFile, linecontent))
		{
			linesCount++;
			curRowsCount = 0;
			std::cout << "Line: " << linecontent << std::endl;
			while (std::regex_search(linecontent, reg_match, reg))
			{
				//std::cout<<"match size:"<<reg_match.size()<<std::endl;
				int vl = stoi(reg_match[1]);
				auto itr = std::find(std::begin(targetVector), std::end(targetVector), vl);
				if (itr == targetVector.end())
					targetVector.push_back(vl);
				else
				{
					errorStringStream << "duplicated value: " << vl << " in file: " << fileName;
					retVal.ErrorMsg = errorStringStream.str();
					return retVal;
				}
				curRowsCount++;
				linecontent = reg_match.suffix().str();
			}
			if (prevRowsCount > 0 && prevRowsCount != curRowsCount)
			{
				errorStringStream << "mismatch of rows count in file: " << fileName;
				retVal.ErrorMsg = errorStringStream.str();
				return retVal;
			}
			else
				prevRowsCount = curRowsCount;
		}
		if (prevRowsCount != linesCount)
		{
			errorStringStream << "mismatch of lines count in file: " << fileName;
			retVal.ErrorMsg = errorStringStream.str();
			return retVal;
		}
		inputValuesCount = targetVector.size();
		for (int i = 1; i < inputValuesCount; i++)
		{
			auto itr = std::find(std::begin(targetVector), std::end(targetVector), i);
			if (itr == targetVector.end())
			{
				errorStringStream << "fail on sorting value: " << i << " in file: " << fileName;
				retVal.ErrorMsg = errorStringStream.str();
				return retVal;
			}
		}

	}
	retVal.Values = targetVector;
	return retVal;
}

int ArgsHandler(int argc, char **argv, int &i)
{
	if (strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0)
	{
		helpFlag = 1;
		i += 1;
		return 1;
	}
	if (strcmp("-f", argv[i]) == 0 || strcmp("--file", argv[i]) == 0)
	{
		if (i < argc - 1 && argv[i + 1] != 0) {
			s_FileName = argv[i + 1];
			i += 2;
			return 2;
		}
	}
	return 0;
}

int main(int argc, char** argv)
{
	Fl::scheme("gtk+");

	//
	int i = 1;
	if (Fl::args(argc, argv, i, ArgsHandler) < argc)
	{
		// note the concatenated strings to give a single format string!
		Fl::fatal("error: unknown option: %s\n"
			"usage: %s [options]\n"
			" -h | --help : print help message\n"
			" -f | --file : specify file \n"
			" plus standard fltk options\n",
			argv[i], argv[0]);
	}
	if (helpFlag)
	{
		Fl::fatal("usage: %s [options]\n"
			" -h | --help : print help message\n"
			" -f | --file : specify file \n"
			" plus standard fltk options:\n"
			"%s\n",
			argv[0], Fl::help);
	}
	//

	Fl_Double_Window windowX(0, 0, 800, 600, "Rotate N-Tiles Game");
	Fl_Box lbl(FL_EMBOSSED_BOX, 10, 215, 180, 30, "value from 3 to 9");
	Fl_Int_Input vl(10, 250, 180, 30);
	Fl_Return_Button btn(10, 285, 180, 30, "Ok");
	/*
	Fl_Menu_Bar menu(0, 0, 800, 25);
	menu.add("&File/&Open", "^o", nullptr);
	menu.add("&File/&Save", "^s", nullptr, 0, FL_MENU_DIVIDER);
	*/
	wind = &windowX;
	Mybox Mb(FL_DOWN_BOX, 250, 50, 500);

	if (s_FileName != nullptr)
	{
		auto parseVal = OpenFileAndLaunch(s_FileName);
		if (parseVal.ErrorMsg.empty() == false)
		{
			Fl::error(parseVal.ErrorMsg.c_str());
			btn.callback(callBack, (void*)&vl);
		}
		else
		{
			Mb.SetTilesValue(parseVal.Values);
			vl.deactivate();
			btn.deactivate();
			Mb.active();
			Mb.show();
		}
	}
	else
	{
		btn.callback(callBack, (void*)&vl);
	}
	windowX.end();
	windowX.show();

	return Fl::run();
}
