#include <game/show/game_show.hpp>
#include <game/logic/game_logic.hpp>
#include <game/core/operate_set.hpp>
#include <chrono>

GameShow::GameShow(GameLogic& game_logic, const GameData& game_data)
	:game_logic_(game_logic)
	,game_data_(game_data)
{

}

GameShow::~GameShow()
{

}

void GameShow::run()
{
	int select = option();
	OperateSet::clear();
	if (select < 0)
	{
		return;
	}
	game_logic_.init_board(mine_board, '0');
	game_logic_.init_board(show_board, '*');
	find_mine();
}

void GameShow::adjust_screen()
{
	OperateSet::clear();
	game_set_.close_mouse_mode();
	printf("Ctrl + 滑动鼠标滑轮调整大小\n");
	printf("注意：若实际游戏画面大于程序窗口会出现刷屏哦\n");
	printf("按任意键返回:>\n");
	char ch = getc(stdin);
	game_set_.open_mouse_mode();
	OperateSet::clear();
}

void GameShow::main_menu()
{
	game_set_.open_mouse_mode();
	char a = 0;
	int y = 0;
	int x = 0;
	int exitgame = 0;

	std::string buf;
	char c;

	auto lastClick = std::chrono::steady_clock::now() - std::chrono::seconds(1);
	while (true)
	{
		MouseEvent event;
		OperateSet::cursor(0, 0);
		int n = game_set_.platform_read(&c, 1);
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
				event = game_set_.get_mouse_event(buf);
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
					OperateSet::clear();	// 先清屏
					run();
				}
			}
			if ((y == 2) && (4 <= x && x <= 21))
			{
				adjust_screen();
			}
			if ((y == 3) && (4 <= x && x <= 21))
			{
				OperateSet::clear(); // 先清屏
				printf("%s\n", "退出游戏");
				exitgame = 1;
			}
		}

		if (exitgame)
			break;
	}
	game_set_.close_mouse_mode();
}

int GameShow::option()
{
	game_set_.open_mouse_mode();
	std::string buf;
	char c;
	char a = 0;
	int y = 0;
	int x = 0;
	while (1)
	{
		MouseEvent event;
		OperateSet::cursor(0, 0);
		int n = game_set_.platform_read(&c, 1);
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
				event = game_set_.get_mouse_event(buf);
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
				game_logic_.set_mine_count(10);
				game_logic_.set_board_area(11, 11);
				return 1;
			}
			if ((y == 2) && (4 <= x && x <= 22))
			{
				game_logic_.set_mine_count(40);
				game_logic_.set_board_area(18, 18);
				return 1;
			}
			if ((y == 3) && (4 <= x && x <= 22))
			{
				game_logic_.set_mine_count(99);
				game_logic_.set_board_area(18, 32);
				return 1;
			}
			if ((y == 4) && (4 <= x && x <= 22))
			{
				break;
			}
		}
	}
	game_set_.close_mouse_mode();
	return -1;
}

int GameShow::display(const Array& arr, int y, int x, MouseEvent event)
{
	int FCount = 0;
	// 将游戏时间生命周期延长，防止游戏完成打印为0
	static int time2 = 0;
	int win = game_logic_.get_win();
	int time1 = game_logic_.get_begin_time();
	int col = game_logic_.get_col();
	int row = game_logic_.get_row();
	int mine_count = game_logic_.get_mine_count();

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

	if (FCount > mine_count) // 防止雷数量成为负数
		FCount = mine_count;

	for (i = 1; i <= row; i++)
	{
		printf("|");
		for (j = 1; j <= col; j++) // 打印扫雷内容
		{
			if (i == y && j * 2 == x + 1)
			{
				printf("\033[42m%c\033[0m ", arr[i][j]);
				mouse_operate_game(i, j, FCount, event);
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
	printf("雷:\033[31m%d\033[0m", mine_count - FCount); // 表示当前雷的数量
	for (i = 0; i <= col / 2 - 2; i++)
		printf("--");
	printf("\n");

	return 0;
}

void GameShow::mouse_operate_game(int y, int x, int flag_count, MouseEvent event)
{
	if (event.operate == left_click && event.action == ma_nothing)
	{
		if (game_data_.show[y][x] == '*')                    // 判断输入的坐标是否被占用
		{
			if (game_data_.first_operate)
			{
				game_logic_.set_mine(y, x);
				game_logic_.set_first_operate(0);
			}
			if (game_data_.mine[y][x] == '1')                // 判断输入的坐标是否是雷
			{
				game_logic_.set_win(-9);
			}
			else
			{
				int count = game_logic_.mine_count(y, x);
				game_logic_.set_board_one_char(show_board, y, x, count + '0');
				game_logic_.set_win(game_data_.win + 1);
				game_logic_.spread(y, x);
			}
		}

		if (game_data_.show[y][x] == 'F')
			game_logic_.set_board_one_char(show_board, y, x, '*');

		if (game_data_.show[y][x] >= '1' && game_data_.show[y][x] <= '7')
			game_logic_.number_spread(y, x, game_data_.show[y][x] - '0');

	}
	if (event.operate == right_click && event.action == ma_nothing)
	{
		if (game_data_.show[y][x] == '*' && flag_count < game_data_.mine_count)
			game_logic_.set_board_one_char(show_board, y, x, 'F');
	}
}

void GameShow::find_mine()
{
	game_set_.open_mouse_mode();
	// 注：由于扫雷下标从1开始，在 3.游戏操作 中需要变通一下
	int x = 1; // 横轴移动
	int y = 1; // 纵轴移动
	game_logic_.set_begin_time((int)time(NULL)); // 获取进入游戏的时间
	int mouseOperate = 0;
	game_logic_.set_first_operate(1);
	game_logic_.set_win(0);

	int falseTime = 1;
	int winTime = 1;
	char c = 0;

	std::string buf;
	auto lastClick = std::chrono::steady_clock::now() - std::chrono::seconds(1);
	while (1)
	{
		MouseEvent event;
		OperateSet::cursor(0, 0);
		int n = game_set_.platform_read(&c, 1);
		if (n <= 0) continue;

		// Ctrl+C 在原始模式下不会自动退出，这里手动处理
		if (c == 0x03) break;

		buf += c;

		if (!buf.empty() && buf[0] == '\033')
		{
			if (buf.size() >= 3 && buf[0] == '\033' && buf[1] == '[' && buf[2] == '<' &&
				(buf.back() == 'M' || buf.back() == 'm'))
			{
				event = game_set_.get_mouse_event(buf);
				buf.clear();
			}
			if (buf.size() > 64) buf.clear();
		}
		else
		{
			buf.clear();
		}

		//获取鼠标当前位置
		y = event.mouse_y;
		x = event.mouse_x;
		mouseOperate = event.operate;

		int row = game_data_.row;
		int col = game_data_.col;
		int mine_count = game_data_.mine_count;
		int win = game_data_.win;

		// 当win为负数意思为被雷炸死
		if (win < row * col - mine_count && win >= 0)
		{
			display(game_data_.show, y, x, event);
			if ((y == row + 2) && (3 <= x && x <= 14))
			{
				printf("***\033[41m  %6s    \033[0m***\n", "返回");
				if (event.operate == left_click && event.action == ma_nothing)
				{
					auto now = std::chrono::steady_clock::now();
					if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClick).count() > 166)
					{
						OperateSet::clear();
						break;
					}
				}
			}
			else
				printf("***  %6s    ***\n", "返回");
		}

		if (win >= row * col - mine_count)
		{
			if (winTime)
			{
				OperateSet::clear();
				OperateSet::open_mouse_mode();
				winTime = 0;
			}
			display(game_data_.show, y, x, MouseEvent{});
			printf("**********************\n");
			printf("******  你赢了  ******\n");
			printf("**********************\n");
			if ((y == row + 5) && (5 <= x && x <= 16))
			{
				printf("*****\033[41m  %6s    \033[0m*****\n", "返回");
				if (event.operate == left_click && event.action == ma_nothing)
				{
					auto now = std::chrono::steady_clock::now();
					if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClick).count() > 166)
					{
						OperateSet::clear();
						break;
					}
				}
			}
			else
				printf("*****  %6s    *****\n", "返回");
		}
		if (win < 0)
		{
			if (falseTime)
			{
				OperateSet::clear();
				game_set_.open_mouse_mode();
				falseTime = 0;
			}
			display(game_data_.mine, y, x, MouseEvent{});
			printf("**********************\n");
			printf("**很遗憾，你被炸死了**\n");
			printf("**********************\n");
			if ((y == row + 5) && (5 <= x && x <= 16))
			{
				printf("*****\033[41m  %6s    \033[0m*****\n", "返回");
				if (event.operate == left_click && event.action == ma_nothing)
				{
					auto now = std::chrono::steady_clock::now();
					if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClick).count() > 166)
					{
						OperateSet::clear();
						break;
					}
				}
			}
			else
				printf("*****  %6s    *****\n", "返回");
		}
	}
}