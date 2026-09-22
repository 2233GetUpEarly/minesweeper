#include <game/logic/game_logic.hpp>
#include <game/core/operate_set.hpp>
#include <chrono>

GameLogic::GameLogic(GameData& data)
	:data_(data)
{

}

GameLogic::~GameLogic()
{

}

const GameData& GameLogic::get_data()
{
	return data_;
}

void GameLogic::set_board_area(int row, int col)
{
	this->data_.row = row;
	this->data_.col = col;
	this->data_.rows = row + 2;
	this->data_.cols = col + 2;
	data_.mine = std::vector<std::vector<char>>(data_.rows, std::vector<char>(data_.cols));
	data_.show = std::vector<std::vector<char>>(data_.rows, std::vector<char>(data_.cols));
}

void GameLogic::set_mine_count(int count)
{
	this->data_.mine_count = count;
}

void GameLogic::set_begin_time(int time)
{
	data_.begin_time = time;
}

void GameLogic::set_first_operate(int operate)
{
	data_.first_operate = operate;
}

void GameLogic::set_win(int win)
{
	data_.win = win;
}

void GameLogic::set_board_one_char(MinesWeeperBoard aim, int row, int col, char set)
{
	if (aim == mine_board)
	{
		data_.mine[row][col] = set;
	}
	else
	{
		data_.show[row][col] = set;
	}
}

int GameLogic::get_win()
{
	return data_.win;
}

int GameLogic::get_begin_time()
{
	return data_.begin_time;
}

int GameLogic::get_row()
{
	return data_.row;
}

int GameLogic::get_col()
{
	return data_.col;
}

int GameLogic::get_mine_count()
{
	return data_.mine_count;
}

static void init_one_board(Array& arr, char set)
{
	for (int i = 0; i < arr.size(); ++i)
	{
		for (int j = 0; j < arr[i].size(); ++j)
		{
			arr[i][j] = set;
		}
	}
}

void GameLogic::init_board(MinesWeeperBoard aim, char set)
{
	if (aim == mine_board)
	{
		init_one_board(data_.mine, set);
	}
	else
	{
		init_one_board(data_.show, set);
	}
}

void GameLogic::set_mine(int y, int x)
{
	int count = 0;
	int i = 0;
	int j = 0;
	// 记录输入行和旁边的两行
	int judgeRow[3] = { 0 };
	for (int k = -1, a = 0; k <= 1; k++, a++)
	{
		judgeRow[a] = k + y;
	}

	// 记录输入列和旁边的两列
	int judgeCol[3] = { 0 };
	for (int k = -1, a = 0; k <= 1; k++, a++)
	{
		judgeCol[a] = k + x;
	}

	while (count < data_.mine_count) // 布置雷的数量，数量到达时则跳出循环
	{
		i = rand() % data_.row + 1; // 行

		// 当输入行且旁边两行与布置行不同时
		if (i != judgeRow[0] && i != judgeRow[1] && i != judgeRow[2])
		{
			j = rand() % data_.col + 1; // 列
		}

		// 当输入行或旁边两行与布置行相同时
		else
		{
			do
			{
				j = rand() % data_.col + 1;
				// 若输入列或旁边两列与布置列相同则进入循环
			} while (j == judgeCol[0] || j == judgeCol[1] || j == judgeCol[2]);
		}
		if (data_.mine[i][j] == '0') // 若为 '0'，则放置雷
		{
			data_.mine[i][j] = '1';
			count++;
		}
	}
}

int GameLogic::mine_count(int y, int x)
{
	int count = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (data_.mine[y + i][x + j] == '1')
			{
				count++;
			}
		}
	}

	return count;
}

void GameLogic::spread(int y, int x)
{
	if (data_.show[y][x] == '0')       // 若周围没有雷才进入
	{
		for (int i = -1; i <= 1; i++) // 行
		{
			for (int j = -1; j <= 1; j++) // 列
			{
				if (y + i >= 1 && y + i <= data_.row && x + j >= 1 && x + j <= data_.col) // 防止超出9×9的范围
				{
					if (data_.show[y + i][x + j] == '*')        // 防止反复递归同一个坐标
					{
						int count = mine_count(y + i, x + j);
						data_.show[y + i][x + j] = count + '0'; // 将已经递归过的坐标显示它周围雷的数量，防止反复递归同一个坐标
						data_.win++;     // 增加查找非雷坐标的数量
						spread(y + i, x + j); // 进入下一次递归
					}
				}
			}
		}
	}
}

void GameLogic::number_spread(int y, int x, int mine_number)
{
	int unknown = 0;		// 未知坐标的数量
	int unknown_blank = 0;	// 未知坐标且未插旗的数量
	int flag_count = 0;		// 插旗坐标的数量

	// 记录周围三者的数量
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (data_.show[y + i][x + j] == '*' || data_.show[y + i][x + j] == 'F')
			{
				unknown++;
			}
			if (data_.show[y + i][x + j] == 'F')
			{
				flag_count++;
			}
			if (data_.show[y + i][x + j] == '*')
			{
				unknown_blank++;
			}
		}
	}

	if (unknown > mine_number && flag_count >= mine_number && unknown_blank != 0) // 周围未知的坐标的数量要大于周围雷的数量
	{
		for (int i = -1; i <= 1; i++) // 行
		{
			for (int j = -1; j <= 1; j++) // 列
			{
				// 防止进入边框坐标
				if (y + i >= 1 && y + i <= data_.row && x + j >= 1 && x + j <= data_.col)
				{
					// 数字展开时扫到雷
					if (data_.show[y + i][x + j] == '*' && data_.mine[y + i][x + j] == '1')
					{
						data_.win = -9;
					}
					// 数字展开扫到非雷
					else if (data_.show[y + i][x + j] == '*' && data_.mine[y + i][x + j] == '0')
					{
						int count = mine_count(y + i, x + j);
						data_.show[y + i][x + j] = count + '0';
						data_.win++;
						spread(y + i, x + j);
					}
				}
			}
		}
	}
}

