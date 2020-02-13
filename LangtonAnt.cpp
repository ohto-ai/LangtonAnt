/************************
* ��������-�ο�ά���ٿ� *
* that boy  2018/12/19  *
* ����޸�  2020/2/14   *
************************/

#include <EasyX.h>
#include <string>// DEBUG
#include <ctime>
#include <conio.h>

namespace LangtonAnt
{
	// ����
	struct Direct
	{
		int dx;
		int dy;
		Direct& TurnLeft();
		Direct& TurnRight();
	}
	LEFT{ -1,0 },
		RIGHT{ 1,0 },
		UP{ 0,-1 },
		DOWN{ 0,1 };

	IMAGE BlockMap; // ��ͼͼ��
	int BlockSize;	// �����С
	bool* Map = nullptr;	// ��ͼ���߼���
	// ��ͼ��С���߼���
	int MapWidth;
	int MapHeight;
	// ��ǰ����
	Direct CurrentDirect;
	// ��ǰλ��
	int CurrentX;
	int CurrentY;
	// �Ƿ񳬳��߽磨��ʼ�й��ɣ�
	bool OutMap = false;

	void Init(int, int, int, int, int, Direct = UP);	// ��ʼ��
	const IMAGE& Update();								// �����߼�ͼ��
	void Quit();										// �����������ڴ�
}

int main()
{
	srand((unsigned int)time(0));

	LangtonAnt::Init(1, 800, 600, 400, 300);	// ���÷����С10����ͼ��С800*600����㣨40,30��
	for (long int step = 0; !LangtonAnt::OutMap; ++step)
		LangtonAnt::Update();
	saveimage(_T("map.bmp"), (IMAGE*)&LangtonAnt::Update());
	LangtonAnt::Quit();

	initgraph(800, 600);
	// SetWindowLong(GetHWnd(), GWL_EXSTYLE, GetWindowLong(GetHWnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	// SetLayeredWindowAttributes(GetHWnd(), WHITE, 0, LWA_COLORKEY);
	setaspectratio(3, 3);
	// setorigin(LangtonAnt::CurrentX, LangtonAnt::CurrentY);
	// ��������
	setbkmode(TRANSPARENT);
	setbkcolor(BLACK);
	settextcolor(BLACK);
	settextstyle(10, 0, L"Arial");
	setlinecolor(WHITE);
	setrop2(R2_XORPEN);

	const int WinWidth = getwidth();
	const int WinHeight = getheight();
	const int HalfWinWidth = getwidth() / 2;
	const int HalfWinHeight = getheight() / 2;

	LangtonAnt::Init(1, 800, 600, 400, 300);	// ���÷����С10����ͼ��С800*600����㣨40,30��

	BeginBatchDraw();
	for (long int step = 0; !LangtonAnt::OutMap; ++step)
	{
		cleardevice();
		int CenX = LangtonAnt::CurrentX * 3;
		int CenY = LangtonAnt::CurrentY * 3;
		setorigin(HalfWinWidth - CenX, HalfWinHeight - CenY);

		putimage(0, 0, &LangtonAnt::Update());

		setorigin(0, 0);
		outtextxy(2, 2, (std::to_wstring(step) + L" steps").c_str());// DEBUG
		line(0, HalfWinHeight / 3, WinWidth / 3, HalfWinHeight / 3);
		line(HalfWinWidth / 3, 0, HalfWinWidth / 3, WinHeight / 3);
		rectangle(1, 1, WinWidth / 3 - 2, WinHeight / 3 - 2);

		FlushBatchDraw();
		Sleep(1);
	}
	LangtonAnt::Quit();
	// ������ʾ������
	outtextxy(400 - textwidth(L"Langton's Ant is running in a loop.") / 2, 300 - textheight('L') / 2, L"Langton's Ant is running in a loop.");
	EndBatchDraw();
	(void)_getch();
	closegraph();

	return 0;
}

// ��ʼ����������
void LangtonAnt::Init(int Blocksize, int MapW, int MapH, int StartX, int StartY, Direct StartDirect)
{
	// ��������
	MapWidth = MapW;
	MapHeight = MapH;
	CurrentX = StartX;
	CurrentY = StartY;
	CurrentDirect = StartDirect;
	OutMap = false;

	if (Map != nullptr)
		delete[]Map;

	Map = new bool[MapWidth * MapHeight]();

	BlockSize = Blocksize;
	BlockMap.Resize(MapWidth * BlockSize, MapHeight * BlockSize);

	// ����ͼͼ��
	SetWorkingImage(&BlockMap);
	setbkcolor(WHITE);
	cleardevice();
	SetWorkingImage();
}
// �����߼���ͼ��
const IMAGE& LangtonAnt::Update()
{
	// ����߽�
	if (CurrentX < 0 || CurrentX >= MapWidth || CurrentY < 0 || CurrentY >= MapHeight)
	{
		OutMap = true;
		return BlockMap;
	}

	if ((Map[CurrentX + CurrentY * MapWidth]) ^ (rand() % 10 < 7))// ��ɫ 70% ����
	{
		CurrentDirect.TurnLeft();// ��ת
	}
	else
	{
		CurrentDirect.TurnRight();// ��ת
	}

	// ����ͼ��
	SetWorkingImage(&BlockMap);

	// ת��
	Map[CurrentX + CurrentY * MapWidth] = !Map[CurrentX + CurrentY * MapWidth];
	// �ػ�ײ�
	setfillcolor(Map[CurrentX + CurrentY * MapWidth] ? BLACK : WHITE);
	solidrectangle(CurrentX * BlockSize, CurrentY * BlockSize, (CurrentX + 1) * BlockSize - 1, (CurrentY + 1) * BlockSize - 1);
	// �ƶ�
	CurrentX += CurrentDirect.dx;
	CurrentY += CurrentDirect.dy;
	// ��������
	setfillcolor(RED);
	solidcircle(CurrentX * BlockSize + BlockSize / 2, CurrentY * BlockSize + BlockSize / 2, BlockSize / 3);

	SetWorkingImage();
	return BlockMap;
}
// �˳���������
void LangtonAnt::Quit()
{
	delete[]Map;
	Map = nullptr;
}
// ��ת
LangtonAnt::Direct& LangtonAnt::Direct::TurnLeft()
{
	if (dx < 0 && dy == 0)// ��
		*this = DOWN;
	else if (dx == 0 && dy < 0)// ��
		*this = LEFT;
	else if (dx > 0 && dy == 0)// ��
		*this = UP;
	else if (dx == 0 && dy > 0)// ��
		*this = RIGHT;
	return *this;
}
// ��ת
LangtonAnt::Direct& LangtonAnt::Direct::TurnRight()
{
	if (dx < 0 && dy == 0)// ��
		*this = UP;
	else if (dx == 0 && dy < 0)// ��
		*this = RIGHT;
	else if (dx > 0 && dy == 0)// ��
		*this = DOWN;
	else if (dx == 0 && dy > 0)// ��
		*this = LEFT;
	return *this;
}