#include "game.h"

extern int getMineCount;

void gotoxy(int a, int b)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = a;
	pos.Y = b;
	SetConsoleCursorPosition(handle, pos);
}

void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void initBoard(char** arr, int rows, int cols, char set)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			arr[i][j] = set;
}

int display(char** arr, int row, int col, int y, int x, int* time1)
{
	gotoxy(0, 0);
	int count = 0;
	//游戏进行时间 为   当前时间    减去 进入游戏的时间  
	int time2 = (int)time(NULL) - *time1;
	int i, j;
	for (i = 0; i <= col / 2 - 2; i++)
		printf("--");
	printf("time:%d", time2);
	for (i = 0; i <= col / 2 - 2; i++)
		printf("--");
	printf("\n");

	for (i = 1; i <= row; i++)
	{
		for (j = 1; j <= col; j++) // 打印扫雷内容
		{
			if (i == y && j == x)
			{
				printf("\033[42m%c\033[0m ", arr[i][j]);
				if (arr[i][j] == 'F')
					count++;
			}
			else
			{
				if (arr[i][j] == '*')
					printf("%c ", arr[i][j]);
				if (arr[i][j] == '0')
					printf("\033[30m%c\033[0m ", arr[i][j]); // 黑色
				if (arr[i][j] == '1')
					printf("\033[36m%c\033[0m ", arr[i][j]); // 浅蓝色
				if (arr[i][j] == '2')
					printf("\033[34m%c\033[0m ", arr[i][j]); // 蓝色
				if (arr[i][j] == '3')
					printf("\033[33m%c\033[0m ", arr[i][j]); // 黄色
				if (arr[i][j] == '4')
					printf("\033[31m%c\033[0m ", arr[i][j]); // 红色
				if (arr[i][j] == '5')
					printf("\033[35m%c\033[0m ", arr[i][j]); // 紫色
				if (arr[i][j] == '6')
					printf("%c ", arr[i][j]);
				if (arr[i][j] == '7')
					printf("%c ", arr[i][j]);
				if (arr[i][j] == 'F')
				{
					printf("\033[32m%c\033[0m ", arr[i][j]); // 绿色
					count++;
				}
			}
		}
		printf("\n"); // 换行
	}
	if (count > getMineCount) // 防止雷数量成为负数
		count = getMineCount;
	for (i = 0; i <= col / 2 - 2; i++)
		printf("--");
	printf("雷:\033[31m%d\033[0m", getMineCount - count); // 表示当前雷的数量
	for (i = 0; i <= col / 2 - 2; i++)
		printf("--");
	printf("\n");

	printf("********************操作说明**********************\n");
	printf("**** Esc.返回 a.查找 s.插旗 d.取旗 f.数字展开 ****\n");
	printf("**************************************************\n");
	return count; // 获得当前F的数量
}

void setMine(char** mine, int row, int col, int y, int x)
{
	int count = 0;
	int i, j;
	// 记录输入行和旁边的两行
	int judgeRow[3] = { 0 };
	for (int k = -1, a = 0; k <= 1; k++, a++)
		judgeRow[a] = k + y;

	// 记录输入列和旁边的两列
	int judgeCol[3] = { 0 };
	for (int k = -1, a = 0; k <= 1; k++, a++)
		judgeCol[a] = k + x;

	while (count < getMineCount) // 布置雷的数量，数量到达时则跳出循环
	{
		i = rand() % row + 1; // 行

		// 当输入行且旁边两行与布置行不同时
		if (i != judgeRow[0] && i != judgeRow[1] && i != judgeRow[2])
			j = rand() % col + 1; // 列

		// 当输入行或旁边两行与布置行相同时
		else
		{
			do
			{
				j = rand() % col + 1;
				// 若输入列或旁边两列与布置列相同则进入循环
			} while (j == judgeCol[0] || j == judgeCol[1] || j == judgeCol[2]);
		}
		if (mine[i][j] == '0') // 若为 '0'，则放置雷
		{
			mine[i][j] = '1';
			count++;
		}
	}
}

int mineCount(char** mine, int y, int x)
{
	int count = 0;
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			if (mine[y + i][x + j] == '1')
				count++;

	return count;
}

void findMine(char** mine, char** show, int row, int col)
{
	// 注：由于扫雷下标从1开始，在 3.游戏操作 中需要变通一下
	int x = 1; // 横轴移动
	int y = 1; // 纵轴移动
	int time1 = (int)time(NULL); // 获取进入游戏的时间
	int win = 0;
	int first = 1;
	while (win < row * col - getMineCount && win >= 0) // 当win为负数意思为被雷炸死
	{
		char ch = 0;
		// 1.打印

		int FCount = display(show, row, col, y, x, &time1);

		// 2.输入

		// kbhit()函数是为防止 程序停在getch()函数接收字符时 导致的时间不流逝
		if (_kbhit())
			ch = _getch();

		// 3.游戏操作
		switch (ch)
		{
		case 'a':
			if (show[y][x] == '*')                    // 判断输入的坐标是否被占用
			{
				if (first)
				{
					setMine(mine, row, col, y, x);
					first = 0;
				}
				if (mine[y][x] == '1')                // 判断输入的坐标是否是雷
				{
					win = -9;
				}
				else
				{
					int count = mineCount(mine, y, x);
					show[y][x] = count + '0';
					win++;
					spread(mine, show, y, x, &win, row, col);
				}
			}
			break;
		case 's':
			// 扫雷游戏有一个细节：当旗子数与雷数对应就不可插旗了
			if (show[y][x] == '*' && FCount < getMineCount)
				show[y][x] = 'F';
			break;
		case 'd':
			if (show[y][x] == 'F')
				show[y][x] = '*';
			break;
		case 'f':
			if (show[y][x] >= '1' && show[y][x] <= '7')
				numberSpread(mine, show, y, x, show[y][x] - '0', &win, row, col);
			break;
		case 0x48: // 向下移动
			if (y > 1) // y大于1
				y = y - 1;
			else
				y = row; // y不大于1直接转到row
			break;
		case 0x50: // 向上移动
			if (y < row) // y小与row
				y = y + 1;
			else
				y = 1; // y等于row直接转到1
			break;
		case 0x4b: // 向左移动
			if (x > 1)
				x = x - 1;
			else
				x = col; // x不大于1直接转到col
			break;
		case 0x4d: // 向右移动
			if (x < col)
				x = x + 1;
			else
				x = 1; // x等于col直接转到1
			break;
		case 27:
			system("cls");
			return;
		}
	}
	if (win == row * col - getMineCount)
	{
		printf("**********************\n");
		printf("******  你赢了  ******\n");
		printf("**********************\n");
		printf("按任意键返回:>\n");
		char winTime = _getch();
		system("cls");
	}
	else
	{
		display(mine, row, col, y, x, &time1);
		printf("**********************\n");
		printf("**很遗憾，你被炸死了**\n");
		printf("**********************\n");
		printf("按任意键返回:>\n");
		char falseTime = _getch();
		system("cls");
	}
}

void spread(char** mine, char** show, int y, int x, int* win, int row, int col)
{
	if (show[y][x] == '0')       // 若周围没有雷才进入
	{
		for (int i = -1; i <= 1; i++) // 行
		{
			for (int j = -1; j <= 1; j++) // 列
			{
				if (y + i >= 1 && y + i <= row && x + j >= 1 && x + j <= col) // 防止超出9×9的范围
				{
					if (show[y + i][x + j] == '*')        // 防止反复递归同一个坐标
					{
						int count = mineCount(mine, y + i, x + j);
						show[y + i][x + j] = count + '0'; // 将已经递归过的坐标显示它周围雷的数量，防止反复递归同一个坐标
						(*win)++;     // 增加查找非雷坐标的数量
						spread(mine, show, y + i, x + j, win, row, col); // 进入下一次递归
					}
				}
			}
		}
	}
}

void numberSpread(char** mine, char** show, int y, int x, int num, int* win, int row, int col)
{
	int unknown = 0;     // 未知坐标的数量
	int unknownBlank = 0;// 未知坐标且未插旗的数量
	int FCount = 0;		 // 插旗坐标的数量
	// 记录周围三者的数量
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (show[y + i][x + j] == '*' || show[y + i][x + j] == 'F')
				unknown++;
			if (show[y + i][x + j] == 'F')
				FCount++;
			if (show[y + i][x + j] == '*')
				unknownBlank++;
		}
	}

	if (unknown > num && FCount >= num && unknownBlank != 0) // 周围未知的坐标的数量要大于周围雷的数量
	{
		for (int i = -1; i <= 1; i++) // 行
		{
			for (int j = -1; j <= 1; j++) // 列
			{
				// 防止进入边框坐标
				if (y + i >= 1 && y + i <= row && x + j >= 1 && x + j <= col)
				{
					// 数字展开时扫到雷
					if (show[y + i][x + j] == '*' && mine[y + i][x + j] == '1')
					{
						(*win) = -9;
					}
					// 数字展开扫到非雷
					else if (show[y + i][x + j] == '*' && mine[y + i][x + j] == '0')
					{
						int count = mineCount(mine, y + i, x + j);
						show[y + i][x + j] = count + '0';
						(*win)++;
						spread(mine, show, y + i, x + j, win, row, col);
					}
				}
			}
		}
	}
}

char** apply(int* rows, int* cols)
{
	// 申请二级指针(二维数组)行的数量
	char** arr = (char**)malloc(sizeof(char*) * (*rows));
	if (NULL == arr)
	{
		printf("游戏异常，已退出\n");
		exit(-1);
	}

	// 申请每行之中列的数量
	for (int i = 0; i < *rows; i++)
	{
		arr[i] = (char*)malloc(sizeof(char) * (*cols));
		if (NULL == arr[i])
		{
			printf("游戏异常，已退出\n");
			exit(-1);
		}
	}

	// 返回首元素地址
	return arr;
}

void release(char** arr, int rows, int cols)
{
	// 先释放一级指针空间
	for (int i = 0; i < rows; i++)
		free(arr[i]);
	// 后释放二级指针的空间
	free(arr);
}

int getMine(int num)
{
	switch (num)
	{
	case 1:
		return 10;
	case 2:
		return 40;
	case 3:
		return 99;
	case 4:
		return myGetMine();
	}
	return 0;
}

void myApply(int* rows, int* cols)
{
	printf("***********************************************\n");
	printf("****************** 提前声明：******************\n");
	printf("*自定义行和列建议至少全部都大于3，否者后果自负*\n");
	printf("***********************************************\n");
	printf("请输入行数:>");
	scanf("%d", rows);
	getchar();
	if (*rows < 1)
	{
		*rows = 4;
		printf("行数小于1，自动改成4行\n");
	}
	printf("请输入列数:>");
	scanf("%d", cols);
	getchar();
	if (*cols < 1)
	{
		*cols = 4;
		printf("列数小于1，自动改成4列\n");
	}
	(*rows) = (*rows) + 2;
	(*cols) = (*cols) + 2;
}

int myGetMine()
{
	int n = 0;
	printf("***********************************************\n");
	printf("****************** 提前声明：******************\n");
	printf("******自定义雷的数量建议最多：行 × 列 - 9 *****\n");
	printf("*****************否者后果自负******************\n");
	printf("请布置雷的数量:>");
	scanf("%d", &n);
	getchar();
	if (n < 0)
	{
		printf("数量小于1，改为0");
		n = 0;
	}
	return n;
}
