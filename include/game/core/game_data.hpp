#pragma once

#include <vector>

using Array = std::vector<std::vector<char>>;

enum MinesWeeperBoard
{
	mine_board,
	show_board,
};

struct GameData
{
	Array mine;
	Array show;
	int rows = 0;
	int cols = 0;
	int mine_count = 0;
	int row = 0;
	int col = 0;
	int win = 0;
	int begin_time = 0;
	int first_operate = 0;
};
