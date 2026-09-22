#include <game/ui/game_ui.hpp>
#include <game/logic/game_logic.hpp>
#include <game/core/operate_set.hpp>
#include <chrono>

GameUI::GameUI(GameLogic& game_logic, const GameData& game_data)
	:game_logic_(game_logic)
	, game_data_(game_data)
	, game_show_(MouseOperateFunc{ this })
{

}

GameUI::~GameUI()
{

}

void GameUI::run()
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

void GameUI::adjust_screen()
{
	OperateSet::clear();
	game_set_.close_mouse_mode();
	game_show_.adjust_screen_show();
	char ch = getc(stdin);
	game_set_.open_mouse_mode();
	OperateSet::clear();
}

MouseEvent GameUI::parse_str(std::string& buf)
{
	char c = 0;
	MouseEvent event;

	int n = game_set_.platform_read(&c, 1);
	if (n <= 0)
	{
		return event;
	}

	buf += c;
	// 收到 ESC 开头，尝试解析完整序列
	if (!buf.empty() && buf[0] == '\033')
	{
		// 只有以 ESC [ < 开头且以 M/m 结尾才尝试解析
		if (buf.size() >= 3 
			&& buf[0] == '\033' 
			&& buf[1] == '[' 
			&& buf[2] == '<' 
			&& (buf.back() == 'M' || buf.back() == 'm'))
		{
			event = game_set_.get_mouse_event(buf);
			buf.clear();
		}
		// 容错：如果累积过长仍不成序列，丢弃，防止内存增长
		if (buf.size() > 64) buf.clear();
	}
	else
	{
		buf.clear();
	}
	
	return event;
}

void GameUI::main_menu()
{
	game_set_.open_mouse_mode();
	int y = 0;
	int x = 0;
	int exitgame = 0;

	std::string buf;
	auto last_click = std::chrono::steady_clock::now() - std::chrono::seconds(1);
	while (true)
	{
		MouseEvent event = parse_str(buf);

		OperateSet::cursor(0, 0);
		y = event.mouse_y;
		x = event.mouse_x;

		game_show_.main_menu_show(y, x);

		if (event.operate == left_click && event.action == ma_nothing)
		{
			if ((y == 1) && (4 <= x && x <= 21))
			{
				auto now = std::chrono::steady_clock::now();
				if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_click).count() > 166)
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

int GameUI::option()
{
	std::string buf;
	int y = 0;
	int x = 0;
	while (1)
	{
		MouseEvent event = parse_str(buf);

		OperateSet::cursor(0, 0);
		y = event.mouse_y;
		x = event.mouse_x;

		game_show_.option_menu_show(y, x);

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
	return -1;
}

void GameUI::mouse_operate_game(int y, int x, int flag_count, MouseEvent event)
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

void GameUI::find_mine()
{
	int x = 1; // 横轴移动
	int y = 1; // 纵轴移动
	game_logic_.set_begin_time((int)time(NULL)); // 获取进入游戏的时间
	game_logic_.set_first_operate(1);
	game_logic_.set_win(0);

	std::string buf;
	auto last_click = std::chrono::steady_clock::now() - std::chrono::seconds(1);
	while (1)
	{
		MouseEvent event = parse_str(buf);

		OperateSet::cursor(0, 0);
		y = event.mouse_y;
		x = event.mouse_x;

		int row = game_data_.row;
		int col = game_data_.col;
		int mine_count = game_data_.mine_count;
		int win = game_data_.win;
		int begin_time = game_data_.begin_time;

		if (win < row * col - mine_count && win >= 0)
		{
			game_show_.display(game_data_.show, y, x, win, begin_time, row, col, mine_count, event);
			int ans = game_show_.game_back_show(y, x, row, col, last_click, event);
			if (ans < 0)
			{
				break;
			}
		}

		if (win >= row * col - mine_count)
		{
			game_show_.display(game_data_.show, y, x, win, begin_time, row, col, mine_count, MouseEvent{});
			int ans = game_show_.game_win_show(y, x, row, col, last_click, event);
			if (ans < 0)
			{
				break;
			}
		}
		// 当win为负数意思为被雷炸死
		if (win < 0)
		{
			game_show_.display(game_data_.mine, y, x, win, begin_time, row, col, mine_count, MouseEvent{});
			int ans = game_show_.game_over_show(y, x, row, col, last_click, event);
			if (ans < 0)
			{
				break;
			}
		}
	}
}