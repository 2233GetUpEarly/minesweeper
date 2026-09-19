#include "game.h"

extern int getMineCount;

void initBoard(char** arr, int rows, int cols, char set)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			arr[i][j] = set;
}

void display(char** arr, int row, int col)
{
	int count = 0;
	system("cls");
	int i, j;
	if (row <= 9 && col <= 9)
	{
		for (i = 0; i <= col / 2 - 1; i++)
			printf("--");
		printf("扫雷");
		for (i = 0; i <= col / 2 - 1; i++)
			printf("--");
		printf("\n|");

		for (i = 0; i <= col; i++) // 打印列的数量
			printf("%d|", i);
		printf("\n"); // 换行

		for (i = 1; i <= row; i++)
		{
			printf("|%d|", i); // 打印当前的行

			for (j = 1; j <= col; j++) // 打印扫雷内容
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
			printf("\n"); // 换行
		}

		for (i = 0; i <= col / 2 - 1; i++)
			printf("--");
		printf("雷:\033[31m%d\033[0m", getMineCount - count);
		for (i = 0; i <= col / 2 - 1; i++)
			printf("--");
		printf("\n");

	}
	else
	{
		for (i = 0; i <= col / 2 - 1; i++)
			printf("---");
		printf("扫雷");
		for (i = 0; i <= col / 2 - 1; i++)
			printf("---");
		printf("\n|");

		for (i = 0; i <= col; i++) // 打印列的数量
			printf("%2d|", i);
		printf("\n"); // 换行

		for (i = 1; i <= row; i++)
		{
			printf("|%2d|", i); // 打印当前的行

			for (j = 1; j <= col; j++) // 打印扫雷内容
			{
				if (arr[i][j] == '*')
					printf("%2c ", arr[i][j]);
				if (arr[i][j] == '0')
					printf("\033[30m%2c\033[0m ", arr[i][j]); // 黑色
				if (arr[i][j] == '1')
					printf("\033[36m%2c\033[0m ", arr[i][j]); // 浅蓝色
				if (arr[i][j] == '2')
					printf("\033[34m%2c\033[0m ", arr[i][j]); // 蓝色
				if (arr[i][j] == '3')
					printf("\033[33m%2c\033[0m ", arr[i][j]); // 黄色
				if (arr[i][j] == '4')
					printf("\033[31m%2c\033[0m ", arr[i][j]); // 红色
				if (arr[i][j] == '5')
					printf("\033[35m%2c\033[0m ", arr[i][j]); // 紫色
				if (arr[i][j] == '6')
					printf("%2c ", arr[i][j]);
				if (arr[i][j] == '7')
					printf("%2c ", arr[i][j]);
				if (arr[i][j] == 'F')
				{
					printf("\033[32m%2c\033[0m ", arr[i][j]); // 绿色
					count++;
				}
			}
			printf("\n"); // 换行
		}

		for (i = 0; i <= col / 2 - 1; i++)
			printf("---");
		printf("雷:\033[31m%d\033[0m", getMineCount - count);
		for (i = 0; i <= col / 2 - 1; i++)
			printf("---");
		printf("\n");
	}
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

void select(int n)
{
	if (n == 1)
	{
		printf("************************************************\n");
		printf("**** 0.返回 1.查找 2.插旗 3.取旗 4.数字展开 ****\n");
		printf("************************************************\n");
	}
	else if (n == 2)
	{
		printf("**********************\n");
		printf("******  你赢了  ******\n");
		printf("**********************\n");
	}
	else if (n == 3)
	{
		printf("**********************\n");
		printf("**很遗憾，你被炸死了**\n");
		printf("**********************\n");
	}
}

void operation(char** mine, char** show, int y, int x, int* first, int* win, int row, int col)
{
	int wantDo = 0;
	do
	{
		printf("请输入操作:>");
		scanf("%d", &wantDo);
		switch (wantDo)
		{
		case 1:
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
					display(show, row, col);
				}
				wantDo = 0;
			}
			else
				printf("坐标被占用，请重新输入\n");
			break;

		case 2:
			if (show[y][x] == '*')
			{
				show[y][x] = 'F';
				display(show, row, col);
				wantDo = 0;
			}
			else
				printf("非未知坐标，请重新输入\n");
			break;

		case 3:
			if (show[y][x] == 'F')
			{
				show[y][x] = '*';
				display(show, row, col);
				wantDo = 0;
			}
			else
				printf("非插旗坐标，请重新输入\n");
			break;

		case 4:
			if (show[y][x] >= '1' && show[y][x] <= '7')
			{
				numberSpread(mine, show, y, x, show[y][x] - '0', win, row, col);
				wantDo = 0;
			}
			else
				printf("非有效坐标，请重新选择\n");
			break;

		case 0:
			break;

		default:
			printf("输入错误，请重新输入\n");
			break;
		}
	} while (wantDo);
}

void findMine(char** mine, char** show, int row, int col)
{
	int y = 0;
	int x = 0;
	int win = 0;
	int first = 1;
	while (win < row * col - getMineCount && win >= 0) // 当win为负数意思为被雷炸死
	{
		printf("请输入坐标:>");
		scanf("%d %d", &y, &x);
		if (y >= 1 && y <= row && x >= 1 && x <= col) // 判断输入的坐标是否在9行9列的范围内
		{
			select(1);
			operation(mine, show, y, x, &first, &win, row, col);
		}
		else
			printf("坐标非法，请重新输入\n");
	}
	if (win == row * col - getMineCount)
		select(2);
	else
	{
		display(mine, row, col);
		select(3);
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

	if (unknown > num) // 周围未知的坐标的数量要大于周围雷的数量
	{
		if (FCount >= num) // 周围旗子的数量要大于等与周围雷的数量
		{
			if (unknownBlank != 0) // 周围未知坐标且未插旗的数量不能为零
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
				if (*win > 0)
					display(show, row, col);
			}
			else
				printf("周围没有未知且未插旗的坐标，不能展开\n");
		}
		else
			printf("周围旗子数量小于周围雷的数量\n");
	}
	else
		printf("未知坐标数量不大于周围雷的数量\n");
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
	if (*rows < 1)
	{
		*rows = 4;
		printf("行数小于1，自动改成4行\n");
	}
	printf("请输入列数:>");
	scanf("%d", cols);
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
	if (n < 0)
	{
		printf("数量小于1，改为0");
		n = 0;
	}
	return n;
}