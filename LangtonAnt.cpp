/************************
* 兰顿蚂蚁-参考维基百科 *
* that boy  2018/12/19  *
* 最后修改  2020/2/14   *
************************/

#include <EasyX.h>
#include <string>// DEBUG
#include <ctime>
#include <conio.h>

namespace LangtonAnt
{
	// 方向
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

	IMAGE BlockMap; // 地图图像
	int BlockSize;	// 方块大小
	bool* Map = nullptr;	// 地图（逻辑）
	// 地图大小（逻辑）
	int MapWidth;
	int MapHeight;
	// 当前方向
	Direct CurrentDirect;
	// 当前位置
	int CurrentX;
	int CurrentY;
	// 是否超出边界（开始有规律）
	bool OutMap = false;

	void Init(int, int, int, int, int, Direct = UP);	// 初始化
	const IMAGE& Update();								// 更新逻辑图像
	void Quit();										// 结束，回收内存
}

int main()
{
	srand((unsigned int)time(0));

	LangtonAnt::Init(1, 800, 600, 400, 300);	// 设置方块大小10，地图大小800*600，起点（40,30）
	for (long int step = 0; !LangtonAnt::OutMap; ++step)
		LangtonAnt::Update();
	saveimage(_T("map.bmp"), (IMAGE*)&LangtonAnt::Update());
	LangtonAnt::Quit();

	initgraph(800, 600);
	// SetWindowLong(GetHWnd(), GWL_EXSTYLE, GetWindowLong(GetHWnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	// SetLayeredWindowAttributes(GetHWnd(), WHITE, 0, LWA_COLORKEY);
	setaspectratio(3, 3);
	// setorigin(LangtonAnt::CurrentX, LangtonAnt::CurrentY);
	// 设置字体
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

	LangtonAnt::Init(1, 800, 600, 400, 300);	// 设置方块大小10，地图大小800*600，起点（40,30）

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
	// 居中显示结束语
	outtextxy(400 - textwidth(L"Langton's Ant is running in a loop.") / 2, 300 - textheight('L') / 2, L"Langton's Ant is running in a loop.");
	EndBatchDraw();
	(void)_getch();
	closegraph();

	return 0;
}

// 初始化兰顿蚂蚁
void LangtonAnt::Init(int Blocksize, int MapW, int MapH, int StartX, int StartY, Direct StartDirect)
{
	// 设置数据
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

	// 填充地图图像
	SetWorkingImage(&BlockMap);
	setbkcolor(WHITE);
	cleardevice();
	SetWorkingImage();
}
// 更新逻辑和图像
const IMAGE& LangtonAnt::Update()
{
	// 处理边界
	if (CurrentX < 0 || CurrentX >= MapWidth || CurrentY < 0 || CurrentY >= MapHeight)
	{
		OutMap = true;
		return BlockMap;
	}

	if ((Map[CurrentX + CurrentY * MapWidth]) ^ (rand() % 10 < 7))// 黑色 70% 概率
	{
		CurrentDirect.TurnLeft();// 左转
	}
	else
	{
		CurrentDirect.TurnRight();// 右转
	}

	// 绘制图像
	SetWorkingImage(&BlockMap);

	// 转向
	Map[CurrentX + CurrentY * MapWidth] = !Map[CurrentX + CurrentY * MapWidth];
	// 重绘底部
	setfillcolor(Map[CurrentX + CurrentY * MapWidth] ? BLACK : WHITE);
	solidrectangle(CurrentX * BlockSize, CurrentY * BlockSize, (CurrentX + 1) * BlockSize - 1, (CurrentY + 1) * BlockSize - 1);
	// 移动
	CurrentX += CurrentDirect.dx;
	CurrentY += CurrentDirect.dy;
	// 绘制蚂蚁
	setfillcolor(RED);
	solidcircle(CurrentX * BlockSize + BlockSize / 2, CurrentY * BlockSize + BlockSize / 2, BlockSize / 3);

	SetWorkingImage();
	return BlockMap;
}
// 退出兰顿蚂蚁
void LangtonAnt::Quit()
{
	delete[]Map;
	Map = nullptr;
}
// 左转
LangtonAnt::Direct& LangtonAnt::Direct::TurnLeft()
{
	if (dx < 0 && dy == 0)// 左
		*this = DOWN;
	else if (dx == 0 && dy < 0)// 上
		*this = LEFT;
	else if (dx > 0 && dy == 0)// 右
		*this = UP;
	else if (dx == 0 && dy > 0)// 下
		*this = RIGHT;
	return *this;
}
// 右转
LangtonAnt::Direct& LangtonAnt::Direct::TurnRight()
{
	if (dx < 0 && dy == 0)// 左
		*this = UP;
	else if (dx == 0 && dy < 0)// 上
		*this = RIGHT;
	else if (dx > 0 && dy == 0)// 右
		*this = DOWN;
	else if (dx == 0 && dy > 0)// 下
		*this = LEFT;
	return *this;
}