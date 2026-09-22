#pragma once

#include <game/core/game_data.hpp>

class GameLogic
{
public:

	GameLogic(GameData& data);

	~GameLogic();

	const GameData& get_data();

	void init_board(MinesWeeperBoard aim, char set);

	void set_mine(int y, int x);

	int mine_count(int y, int x);

	void spread(int y, int x);

	void number_spread(int y, int x, int mine_number);

public:

	void set_board_area(int row, int col);

	void set_mine_count(int count);

	void set_begin_time(int time);

	void set_first_operate(int operate);

	void set_win(int win);

	void set_board_one_char(MinesWeeperBoard aim, int row, int col, char set);

	int get_win();

	int get_begin_time();

	int get_row();
	
	int get_col();

	int get_mine_count();

private:

	GameData& data_;
};
