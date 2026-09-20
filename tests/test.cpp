#include "game.hpp"
#include <set.hpp>
#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

int getMineCount = 0;

void adjustScreen()
{
	GameSet::clear();
	printf("Ctrl + 滑动鼠标滑轮调整大小\n");
	printf("注意：若实际游戏画面大于程序窗口会出现刷屏哦\n");
	printf("按任意键返回:>\n");
	char ch = _getch();
	GameSet::clear();
}

GameSet gs;

void menu1()
{
	gs.open_mouse_mode();
	char a = 0;
	int y = 0;
	int x = 0;
	int exitgame = 0;

	int judge = 1;

	std::string buf;
	char c;

	auto lastClick = std::chrono::steady_clock::now() - std::chrono::seconds(1);
	while (true)
	{
		MouseEvent event;
		GameSet::cursor(0, 0);
		int n = gs.platform_read(&c, 1);
		//int n = platformRead(&c, 1);
		if (n <= 0) continue;

		// Ctrl+C 在原始模式下不会自动退出，这里手动处理
		if (c == 0x03) break;

		buf += c;

		// 收到 ESC 开头，尝试解析完整序列
		if (!buf.empty() && buf[0] == '\033')
		{
			// 只有以 ESC [ < 开头且以 M/m 结尾才尝试解析
			if (buf.size() >= 3 && buf[0] == '\033' && buf[1] == '[' && buf[2] == '<' &&
				(buf.back() == 'M' || buf.back() == 'm'))
			{
				event = gs.get_mouse_event(buf);
				//std::cout << event.mouse_x << " " << event.mouse_y << std::endl;
				buf.clear();
			}
			// 容错：如果累积过长仍不成序列，丢弃，防止内存增长
			if (buf.size() > 64) buf.clear();
		}
		else
		{
			// 普通按键，直接清掉
			buf.clear();
		}
		y = event.mouse_y;
		x = event.mouse_x;

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

		int mouseOperate = event.operate;

		if (event.operate == left_click && event.action == ma_nothing)
		{
			if ((y == 1) && (4 <= x && x <= 21))
			{
				auto now = std::chrono::steady_clock::now();
				if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClick).count() > 166)
				{
					// 太近了，忽略
					GameSet::clear();	// 先清屏
					game();
				}
			}
			if ((y == 2) && (4 <= x && x <= 21))
			{
				adjustScreen();
				judge = 0;
			}
			if ((y == 3) && (4 <= x && x <= 21))
			{
				GameSet::clear(); // 先清屏
				printf("%s\n", "退出游戏");
				exitgame = 1;
			}
		}

		if (exitgame)
			break;
	}
	gs.close_mouse_mode();
}

int option(int* rows, int* cols)
{
	gs.open_mouse_mode();
	std::string buf;
	char c;
	char a = 0;
	int y = 0;
	int x = 0;
	while (1)
	{
		MouseEvent event;
		GameSet::cursor(0, 0);
		int n = gs.platform_read(&c, 1);
		//int n = platformRead(&c, 1);
		if (n <= 0) continue;

		// Ctrl+C 在原始模式下不会自动退出，这里手动处理
		if (c == 0x03) break;

		buf += c;

		// 收到 ESC 开头，尝试解析完整序列
		if (!buf.empty() && buf[0] == '\033')
		{
			// 只有以 ESC [ < 开头且以 M/m 结尾才尝试解析
			if (buf.size() >= 3 && buf[0] == '\033' && buf[1] == '[' && buf[2] == '<' &&
				(buf.back() == 'M' || buf.back() == 'm'))
			{
				event = gs.get_mouse_event(buf);
				buf.clear();
			}
			// 容错：如果累积过长仍不成序列，丢弃，防止内存增长
			if (buf.size() > 64) buf.clear();
		}
		else
		{
			// 普通按键，直接清掉
			buf.clear();
		}
		y = event.mouse_y;
		x = event.mouse_x;

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

		if (event.operate == left_click && event.action == ma_nothing)
		{
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
		}
	}
	gs.close_mouse_mode();
	return 1;
}

void game()
{
	int rows = 0;
	int cols = 0;
	int num = option(&rows, &cols);
	GameSet::clear();
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
	game_release(mine, rows, cols);
	game_release(show, rows, cols);
}

void test()
{
	GameSet::hide_cursor();
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

