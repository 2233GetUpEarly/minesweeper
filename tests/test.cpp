#include "game.hpp"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

int getMineCount = 0;

//定义句柄变量
HANDLE get1;
//权限准备
DWORD get2 = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
//定义输入事件结构体
INPUT_RECORD mouseRecord;
//用于存储读取记录
DWORD res;

void getPower() // 获取权限
{
	if (!SetConsoleMode(get1, get2))
		fprintf(stderr, "%s\n", "SetConsoleMode");
}

void adjustScreen()
{
	system("cls");
	printf("Ctrl + 滑动鼠标滑轮调整大小\n");
	printf("注意：若实际游戏画面大于程序窗口会出现刷屏哦\n");
	printf("按任意键返回:>\n");
	char ch = _getch();
	system("cls");
}

void menu1()
{
	getPower();
	char a = 0;
	int y = 0;
	int x = 0;
	int exitgame = 0;

	int judge = 1;

	while (1)
	{
		gotoxy(0, 0);

		//读取输入事件
		ReadConsoleInput(get1, &mouseRecord, 1, &res);
		//获取鼠标当前位置
		y = mouseRecord.Event.MouseEvent.dwMousePosition.Y;
		x = mouseRecord.Event.MouseEvent.dwMousePosition.X;

		char arr[3][13] = { "开始游戏", "调整画面", "退出游戏" };
		printf("*************************\n");
		if ((y == 1) && (4 <= x && x <= 21))
		{
			printf("****\033[41m     %-10s  \033[0m****\n", &arr[0][0]);
		}
		else
			printf("****   %-12s  ****\n", &arr[0][0]);
		if ((y == 2) && (4 <= x && x <= 21))
		{
			printf("****\033[41m     %-10s  \033[0m****\n", &arr[1][0]);
		}
		else
			printf("****   %-12s  ****\n", &arr[1][0]);

		if ((y == 3) && (4 <= x && x <= 21))
		{
			printf("****\033[41m     %-10s  \033[0m****\n", &arr[2][0]);
		}
		else
			printf("****   %-12s  ****\n", &arr[2][0]);

		printf("*************************\n");
		printf("若点一下无反应可重新点一下\n");

		int mouseOperate = 0;
		// 利用GetAsyncKeyState和judge变量可以在getch()得到字符后
		// 对鼠标异常进行标记，使下一次GetAsyncKeyState
		// 不会对异常信息进行处理，这时只需再点击左键，
		// 代码会获取新的信息，避免异常信息停留。
		if (judge)
			mouseOperate = mouseRecord.Event.MouseEvent.dwButtonState;
		else if (GetAsyncKeyState(VK_LBUTTON))
			judge = 1;
		else
			mouseOperate = 0;

		if (mouseRecord.EventType == MOUSE_EVENT)
		{
			switch (mouseOperate)
			{
			case FROM_LEFT_1ST_BUTTON_PRESSED:
				if ((y == 1) && (4 <= x && x <= 21))
				{
					system("cls"); // 先清屏
					game();
					getPower();
				}
				if ((y == 2) && (4 <= x && x <= 21))
				{
					adjustScreen();
					judge = 0;
					getPower();
				}
				if ((y == 3) && (4 <= x && x <= 21))
				{
					system("cls");// 先清屏
					printf("%s\n", "退出游戏");
					exitgame = 1;
				}
				break;
			}
		}
		// 利用GetAsyncKeyState和judge变量可以在getch()得到字符后
		// 对鼠标异常进行标记，使下一次GetAsyncKeyState
		// 不会对异常信息进行处理，这时只需再点击左键，
		// 代码会获取新的信息，避免异常信息停留。
		GetAsyncKeyState(VK_LBUTTON);
		Sleep(50);
		if (exitgame)
			break;
	}
}

int option(int* rows, int* cols)
{
	GetAsyncKeyState(VK_LBUTTON);
	getPower();
	char a = 0;
	int y = 0;
	int x = 0;
	while (1)
	{
		gotoxy(0, 0); // 固定画面

		//读取输入事件
		ReadConsoleInput(get1, &mouseRecord, 1, &res);
		//获取鼠标当前位置
		y = mouseRecord.Event.MouseEvent.dwMousePosition.Y;
		x = mouseRecord.Event.MouseEvent.dwMousePosition.X;

		char arr[4][20] = { "1.初级(9×9)", "2.中级(16×16)", "3.高级(16×30)", "返回" };

		printf("*************************\n");
		if ((y == 1) && (4 <= x && x <= 22))
			printf("****\033[41m   %-15s\033[0m****\n", &arr[0][0]);
		else
			printf("****   %-15s****\n", &arr[0][0]);
		if ((y == 2) && (4 <= x && x <= 22))
			printf("****\033[41m   %-15s\033[0m****\n", &arr[1][0]);
		else
			printf("****   %-15s****\n", &arr[1][0]);
		if ((y == 3) && (4 <= x && x <= 22))
			printf("****\033[41m   %-15s\033[0m****\n", &arr[2][0]);
		else
			printf("****   %-15s****\n", &arr[2][0]);
		if ((y == 4) && (4 <= x && x <= 22))
			printf("****\033[41m   %-14s\033[0m****\n", &arr[3][0]);
		else
			printf("****   %-14s****\n", &arr[3][0]);
		printf("*************************\n");

		int mouseOperate = mouseRecord.Event.MouseEvent.dwButtonState;

		if (mouseRecord.EventType == MOUSE_EVENT)
		{
			switch (mouseOperate)
			{
			case FROM_LEFT_1ST_BUTTON_PRESSED:
				if ((y == 1) && (4 <= x && x <= 22))
				{
					*rows = 11;
					*cols = 11;
					return 1;
				}
				if ((y == 2) && (4 <= x && x <= 22))
				{
					*rows = 18;
					*cols = 18;
					return 2;
				}
				if ((y == 3) && (4 <= x && x <= 22))
				{
					*rows = 18;
					*cols = 32;
					return 3;
				}
				if ((y == 4) && (4 <= x && x <= 22))
				{
					return -1;
				}
				break;
			}
		}
		Sleep(50);
	}
	return 1;
}

void game()
{
	int rows = 0;
	int cols = 0;
	int num = option(&rows, &cols);
	system("cls");
	if (num < 0)
		return;
	char** mine = apply(&rows, &cols);
	char** show = apply(&rows, &cols);
	getMineCount = 0;
	getMineCount = getMine(num);
	int row = rows - 2;
	int col = cols - 2;
	initBoard(mine, rows, cols, '0');
	initBoard(show, rows, cols, '*');
	findMine(mine, show, row, col);
	release(mine, rows, cols);
	release(show, rows, cols);
}

void test()
{
	get1 = GetStdHandle(STD_INPUT_HANDLE);
	HideCursor();
	srand((unsigned int)time(NULL)); // 使rand函数产生伪随机数
	menu1();
	CloseHandle(get1);
}

int main()
{
#ifdef _WIN32
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
#endif

	test();
	return 0;
}

