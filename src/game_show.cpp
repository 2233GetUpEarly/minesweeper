#include <game/ui/game_show.hpp>
#include <game/ui/game_ui.hpp>

#include <cstdio>
#include <ctime>
#include <cassert>
#include <chrono>

void MouseOperateFunc::operator()(int y, int x, int flag_count, MouseEvent event)
{
	ui->mouse_operate_game(y, x, flag_count, event);
}

GameShow::GameShow(MouseOperateFunc mouse_operate_func)
	:mouse_operate_func_(mouse_operate_func)
{
	;
}

void GameShow::main_menu_show(int y, int x)
{
	char arr[3][13] = { "开始游戏", "调整画面", "退出游戏" };
	printf("*************************\n");
	if ((y == 1) && (4 <= x && x <= 21))	printf("****\033[41m     %-10s  \033[0m****\n", &arr[0][0]);
	else									printf("****   %-12s  ****\n", &arr[0][0]);

	if ((y == 2) && (4 <= x && x <= 21))	printf("****\033[41m     %-10s  \033[0m****\n", &arr[1][0]);
	else									printf("****   %-12s  ****\n", &arr[1][0]);

	if ((y == 3) && (4 <= x && x <= 21))	printf("****\033[41m     %-10s  \033[0m****\n", &arr[2][0]);
	else									printf("****   %-12s  ****\n", &arr[2][0]);
	printf("*************************\n");
	printf("若点一下无反应可重新点一下\n");
}

void GameShow::adjust_screen_show()
{
	printf("Ctrl + 滑动鼠标滑轮调整大小\n");
	printf("注意：若实际游戏画面大于程序窗口会出现刷屏哦\n");
	printf("按任意键返回:>\n");
}

void GameShow::option_menu_show(int y, int x)
{
	char arr[4][20] = { "1.初级(9×9)", "2.中级(16×16)", "3.高级(16×30)", "返回" };

	printf("*************************\n");
	if ((y == 1) && (4 <= x && x <= 22))	printf("****\033[41m   %-15s\033[0m****\n", &arr[0][0]);
	else									printf("****   %-15s****\n", &arr[0][0]);

	if ((y == 2) && (4 <= x && x <= 22))	printf("****\033[41m   %-15s\033[0m****\n", &arr[1][0]);
	else									printf("****   %-15s****\n", &arr[1][0]);

	if ((y == 3) && (4 <= x && x <= 22))	printf("****\033[41m   %-15s\033[0m****\n", &arr[2][0]);
	else									printf("****   %-15s****\n", &arr[2][0]);
	
	if ((y == 4) && (4 <= x && x <= 22))	printf("****\033[41m   %-14s\033[0m****\n", &arr[3][0]);
	else									printf("****   %-14s****\n", &arr[3][0]);
	printf("*************************\n");
}

static void print_frame_up_col(int col, int time)
{
	for (int i = 0; i <= col / 2 - 2; ++i)	printf("--");
	printf("time:%-3d", time);
	for (int i = 0; i <= col / 2 - 3; ++i)	printf("--");

	printf("\n");
}

static void print_frame_down_col(int col, int flag_count, int mine_count)
{
	for (int i = 0; i <= col / 2 - 2; ++i)	printf("--");
	printf("雷:\033[31m%d\033[0m", mine_count - flag_count); // 表示当前雷的数量
	for (int i = 0; i <= col / 2 - 2; ++i)	printf("--");

	printf("\n");
}

static void print_plaid(char set)
{
	switch (set)
	{
	case '*':
		printf("%c ", set);
		break;
	case '0':
		printf("\033[30m%c\033[0m ", set); // 黑色
		break;
	case '1':
		printf("\033[36m%c\033[0m ", set); // 浅蓝色
		break;
	case '2':
		printf("\033[34m%c\033[0m ", set); // 蓝色
		break;
	case '3':
		printf("\033[33m%c\033[0m ", set); // 黄色
		break;
	case '4':
		printf("\033[31m%c\033[0m ", set); // 红色
		break;
	case '5':
		printf("\033[35m%c\033[0m ", set); // 紫色
		break;
	case '6':
		printf("%c ", set);
		break;
	case '7':
		printf("%c ", set);
		break;
	case 'F':
		printf("\033[32m%c\033[0m ", set); // 绿色
		break;
	default:
		assert(false);
		break;
	}
}

void GameShow::display(const Array& arr, int y, int x, int win, int begin_time, int row, int col, int mine_count, MouseEvent event)
{
	int FCount = 0;
	int time2 = 0;

	if (win != NULL)
	{
		time2 = (int)time(NULL) - begin_time;
	}

	print_frame_up_col(col, time2);

	for (int i = 1; i <= row; ++i)
	{
		for (int j = 1; j <= col; ++j)
		{
			if (arr[i][j] == 'F')
			{
				FCount++;
			}
		}
	}

	if (FCount > mine_count) // 防止雷数量成为负数
	{
		FCount = mine_count;
	}
	for (int i = 1; i <= row; ++i)
	{
		printf("|");
		for (int j = 1; j <= col; ++j) // 打印扫雷内容
		{
			if (i == y && j * 2 == x + 1)
			{
				printf("\033[42m%c\033[0m ", arr[i][j]);
				mouse_operate_func_(i, j, FCount, event);
			}
			else
			{
				print_plaid(arr[i][j]);
			}
		}
		printf("\n"); // 换行
	}

	print_frame_down_col(col, FCount, mine_count);
}

int GameShow::game_over_show(int y, int x, int row, int col, std::chrono::steady_clock::time_point last_click, MouseEvent event)
{
	printf("**********************\n");
	printf("**很遗憾，你被炸死了**\n");
	printf("**********************\n");
	if ((y == row + 5) && (5 <= x && x <= 16))
	{
		printf("*****\033[41m  %6s    \033[0m*****\n", "返回");
		if (event.operate == left_click && event.action == ma_nothing)
		{
			auto now = std::chrono::steady_clock::now();
			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_click).count() > 166)
			{
				OperateSet::clear();
				return -1;
			}
		}
	}
	else
	{
		printf("*****  %6s    *****\n", "返回");
	}
	return 0;
}

int GameShow::game_win_show(int y, int x, int row, int col, std::chrono::steady_clock::time_point last_click, MouseEvent event)
{
	printf("**********************\n");
	printf("******  你赢了  ******\n");
	printf("**********************\n");
	if ((y == row + 5) && (5 <= x && x <= 16))
	{
		printf("*****\033[41m  %6s    \033[0m*****\n", "返回");
		if (event.operate == left_click && event.action == ma_nothing)
		{
			auto now = std::chrono::steady_clock::now();
			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_click).count() > 166)
			{
				OperateSet::clear();
				return -1;
			}
		}
	}
	else
	{
		printf("*****  %6s    *****\n", "返回");
	}
	return 0;
}

int GameShow::game_back_show(int y, int x, int row, int col, std::chrono::steady_clock::time_point last_click, MouseEvent event)
{
	if ((y == row + 2) && (3 <= x && x <= 14))
	{
		printf("***\033[41m  %6s    \033[0m***\n", "返回");
		if (event.operate == left_click && event.action == ma_nothing)
		{
			auto now = std::chrono::steady_clock::now();
			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_click).count() > 166)
			{
				OperateSet::clear();
				return -1;
			}
		}
	}
	else
	{
		printf("***  %6s    ***\n", "返回");
	}
	return 0;
}
