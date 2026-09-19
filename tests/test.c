#include "game.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

int getMineCount = 0;

void menu1()
{
	char a = 0;
	int y = 0; // 只限纵轴移动
	while (1)
	{
		gotoxy(0, 0);
		// 1.打印
		char arr[2][13] = { "开始游戏", "退出游戏" };
		printf("*************************\n");
		if (y == 0)
			printf("****   \033[41m->%-10s  \033[0m****\n", &arr[0][0]);
		else
			printf("****   %-12s  ****\n", &arr[0][0]);
		if (y == 1)
			printf("****   \033[41m->%-10s  \033[0m****\n", &arr[1][0]);
		else
			printf("****   %-12s  ****\n", &arr[1][0]);
		printf("*************************\n");
		printf("操作说明:>\n");
		printf("上下方向键选择\n");
		printf("小写字母a确定键\n");
		// 2.读取
		a = _getch();
		// 3.选择
		switch (a)
		{
		case 0x48:
			y = (y - 1 + 2) % 2;
			break;
		case 0x50:
			y = (y + 1 + 2) % 2;
			break;
		case 'a':
			if (y == 0)
			{
				system("cls"); // 先清屏
				game();
			}
			else
			{
				system("cls");// 先清屏
				printf("%s\n", "退出游戏");
				exit(0);
			}
			break;
		}
	}
}

int option(int* rows, int* cols)
{
	char a = 0;
	int y = 0; // 只限纵轴移动
	while (1)
	{
		gotoxy(0, 0); // 固定画面
		// 1.打印画面
		char arr[4][20] = { "1.初级(9×9)", "2.中级(16×16)", "3.高级(16×30)", "4.自定义" };

		printf("*************************\n");
		if (y == 0)
			printf("****   \033[41m%-15s\033[0m****\n", &arr[0][0]);
		else
			printf("****   %-15s****\n", &arr[0][0]);
		if (y == 1)
			printf("****   \033[41m%-15s\033[0m****\n", &arr[1][0]);
		else
			printf("****   %-15s****\n", &arr[1][0]);
		if (y == 2)
			printf("****   \033[41m%-15s\033[0m****\n", &arr[2][0]);
		else
			printf("****   %-15s****\n", &arr[2][0]);
		if (y == 3)
			printf("****   \033[41m%-14s\033[0m****\n", &arr[3][0]);
		else
			printf("****   %-14s****\n", &arr[3][0]);
		printf("*************************\n");
		printf("操作说明:>\n");
		printf("上下方向键选择\n");
		printf("小写字母a确定键\n");
		// 2.输入字符
		a = _getch();
		// 3.移动操作
		switch (a)
		{
		case 0x48:
			y = (y - 1 + 4) % 4;
			break;
		case 0x50:
			y = (y + 1 + 4) % 4;
			break;
		case 'a':
			if (y == 0)
			{
				*rows = 11;
				*cols = 11;
				return 1;
			}
			else if (y == 1)
			{
				*rows = 18;
				*cols = 18;
				return 2;
			}
			else if (y == 2)
			{
				*rows = 18;
				*cols = 32;
				return 3;
			}
			else if (y == 3)
			{
				system("cls");// 先清屏
				myApply(rows, cols);
				return 4;
			}
			break;
		}
	}
	return 1;
}

void game()
{
	int rows = 0;
	int cols = 0;
	int num = option(&rows, &cols);
	char** mine = apply(&rows, &cols);
	char** show = apply(&rows, &cols);
	getMineCount = getMine(num);
	int row = rows - 2;
	int col = cols - 2;
	initBoard(mine, rows, cols, '0');
	initBoard(show, rows, cols, '*');
	system("cls");// 先清屏
	findMine(mine, show, row, col);
	release(mine, rows, cols);
	release(show, rows, cols);
}

void test()
{
	HideCursor();
	srand((unsigned int)time(NULL)); // 使rand函数产生伪随机数
	menu1();
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
