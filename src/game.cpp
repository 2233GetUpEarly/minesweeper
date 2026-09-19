#include "game.hpp"
#include <set.hpp>

extern int getMineCount;
extern HANDLE get1;
extern DWORD get2;
extern INPUT_RECORD mouseRecord;
extern DWORD res;

void initBoard(char** arr, int rows, int cols, char set)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			arr[i][j] = set;
}

int display(char** arr, char** mine, char** show, int row, int col, int y, int x, int time1, int* first, int* win, int mouseOperate)
{
	int FCount = 0;
	// 将游戏时间生命周期延长，防止游戏完成打印为0
	static int time2 = 0;
	if (win != NULL)
	{
		//游戏进行时间 为   当前时间    减去 进入游戏的时间
		time2 = (int)time(NULL) - time1;
		// 遗憾的是我不知道第一种鼠标操作有没有
		// 同khbit()一样有非阻塞函数对鼠标进行监听
	}
	int i, j;
	for (i = 0; i <= col / 2 - 2; i++)
		printf("--");
	printf("time:%-3d", time2);
	for (i = 0; i <= col / 2 - 3; i++)
		printf("--");
	printf("\n");

	for (i = 1; i <= row; i++)
		for (j = 1; j <= col; j++)
			if (arr[i][j] == 'F')
				FCount++;

	if (FCount > getMineCount) // 防止雷数量成为负数
		FCount = getMineCount;

	for (i = 1; i <= row; i++)
	{
		printf("|");
		for (j = 1; j <= col; j++) // 打印扫雷内容
		{
			if (i == y && j * 2 == x + 1)
			{
				printf("\033[42m%c\033[0m ", arr[i][j]);
				if (mouseRecord.EventType == MOUSE_EVENT)
				{
					mouseOperateGame(mine, show, mouseOperate, i, j, row, col, first, win, FCount);
				}
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
					printf("\033[32m%c\033[0m ", arr[i][j]); // 绿色
			}
		}
		printf("\n"); // 换行
	}

	for (i = 0; i <= col / 2 - 2; i++)
		printf("--");
	printf("雷:\033[31m%d\033[0m", getMineCount - FCount); // 表示当前雷的数量
	for (i = 0; i <= col / 2 - 2; i++)
		printf("--");
	printf("\n");

	return 0;
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

void mouseOperateGame(char** mine, char** show, int mouseOperate, int y, int x, int row, int col, int* first, int* win, int FCount)
{
	switch (mouseOperate)
	{
	case  FROM_LEFT_1ST_BUTTON_PRESSED:
		if (show[y][x] == '*')                    // 判断输入的坐标是否被占用
		{
			if (*first)
			{
				setMine(mine, row, col, y, x);
				*first = 0;
			}
			if (mine[y][x] == '1')                // 判断输入的坐标是否是雷
			{
				*win = -9;
			}
			else
			{
				int count = mineCount(mine, y, x);
				show[y][x] = count + '0';
				(*win)++;
				spread(mine, show, y, x, win, row, col);
			}
		}

		if (show[y][x] == 'F')
			show[y][x] = '*';

		if (show[y][x] >= '1' && show[y][x] <= '7')
			numberSpread(mine, show, y, x, show[y][x] - '0', win, row, col);

		break;
	case  RIGHTMOST_BUTTON_PRESSED:
		if (show[y][x] == '*' && FCount < getMineCount)
			show[y][x] = 'F';
		break;
	}
}

void findMine(char** mine, char** show, int row, int col)
{
	getPower();
	// 注：由于扫雷下标从1开始，在 3.游戏操作 中需要变通一下
	int x = 1; // 横轴移动
	int y = 1; // 纵轴移动
	int time1 = (int)time(NULL); // 获取进入游戏的时间
	int win = 0;
	int mouseOperate = 0;
	int first = 1;

	int falseTime = 1;
	int winTime = 1;

	while (1)
	{
		GameSet::cursor(0, 0);

		ReadConsoleInput(get1, &mouseRecord, 1, &res);
		//获取鼠标当前位置
		y = mouseRecord.Event.MouseEvent.dwMousePosition.Y;
		x = mouseRecord.Event.MouseEvent.dwMousePosition.X;

		mouseOperate = mouseRecord.Event.MouseEvent.dwButtonState;

		// 当win为负数意思为被雷炸死
		if (win < row * col - getMineCount && win >= 0)
		{
			display(show, mine, show, row, col, y, x, time1, &first, &win, mouseOperate);
			if ((y == row + 2) && (3 <= x && x <= 14))
			{
				printf("***\033[41m  %6s    \033[0m***\n", "返回");
				if (mouseOperate == FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					GameSet::clear();
					break;
				}
			}
			else
				printf("***  %6s    ***\n", "返回");
		}

		if (win >= row * col - getMineCount)
		{
			if (winTime)
			{
				GameSet::clear();
				getPower();
				winTime = 0;
			}
			display(show, mine, show, row, col, y, x, time1, NULL, NULL, 0);
			printf("**********************\n");
			printf("******  你赢了  ******\n");
			printf("**********************\n");
			if ((y == row + 5) && (5 <= x && x <= 16))
			{
				printf("*****\033[41m  %6s    \033[0m*****\n", "返回");
				if (mouseOperate == FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					GameSet::clear();
					break;
				}
			}
			else
				printf("*****  %6s    *****\n", "返回");
		}
		if (win < 0)
		{
			if (falseTime)
			{
				GameSet::clear();
				getPower();
				falseTime = 0;
			}
			display(mine, mine, show, row, col, y, x, time1, NULL, NULL, 0);
			printf("**********************\n");
			printf("**很遗憾，你被炸死了**\n");
			printf("**********************\n");
			if ((y == row + 5) && (5 <= x && x <= 16))
			{
				printf("*****\033[41m  %6s    \033[0m*****\n", "返回");
				if (mouseOperate == FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					GameSet::clear();
					break;
				}
			}
			else
				printf("*****  %6s    *****\n", "返回");
		}
		Sleep(100);
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
	}
	return 0;
}
// 这里删除自定义操作，一是怕出现Bug，二是懒得去改。
