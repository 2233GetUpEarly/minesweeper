#pragma once

#include <game/ui/game_show.hpp>

#include <game/core/game_data.hpp>
#include <game/core/operate_set.hpp>

#include <utility>

class GameLogic;

class GameUI
{
public:

	GameUI(GameLogic&, const GameData&);

	~GameUI();

	void run();

	void adjust_screen();

	void main_menu();

	int option();

	void find_mine();

	void mouse_operate_game(int y, int x, int flag_count, MouseEvent event);

private:

	MouseEvent parse_str(std::string& buf);

private:

	OperateSet game_set_;
	GameLogic& game_logic_;
	const GameData& game_data_;
	GameShow game_show_;
};

