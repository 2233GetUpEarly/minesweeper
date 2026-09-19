#include "game.h"

int getMineCount = 0;

void menu(int n)
{
	if (n == 1)
	{
		printf("************************\n");
		printf("**** 1.play  0.exit ****\n");
		printf("************************\n");
	}
	else if (n == 2)
	{
		printf("*************************\n");
		printf("****   1.初级(9×9)   ****\n");
		printf("****   2.中级(16×16) ****\n");
		printf("****   3.高级(16×30) ****\n");
		printf("****   4.自定义      ****\n");
		printf("*************************\n");
	}
}

int option(int* rows, int* cols)
{
	menu(2);
	int choose = 0;
	do
	{
		printf("请选择难度:>");
		scanf("%d", &choose);
		switch (choose)
		{
		case 1:
			*rows = 11;
			*cols = 11;
			return 1;
		case 2:
			*rows = 18;
			*cols = 18;
			return 2;
		case 3:
			*rows = 18;
			*cols = 32;
			return 3;
		case 4:
			myApply(rows, cols);
			return 4;
		default:
			printf("输入错误，请重新输入\n");
			break;
		}
	} while (choose);
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
	display(show, row, col);
	findMine(mine, show, row, col);
	release(mine, rows, cols);
	release(show, rows, cols);
}

void test()
{
	int input = 0;
	srand((unsigned int)time(NULL)); // 使rand函数产生伪随机数
	do
	{
		menu(1);
		printf("请输入操作:>");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			game();
			break;
		case 0:
			printf("退出游戏\n");
			break;
		default:
			printf("输入错误，请重新输入\n");
			break;
		}
	} while (input);
}

int main()
{
	test();
	return 0;
}