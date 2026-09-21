#pragma once

#include <game/game.hpp>
#include <game/core/operate_set.hpp>

class GameShow
{
public:

	GameShow(Game&);

	~GameShow();

	void run();

	void adjust_screen();

	void main_menu();

	int option();

	int display(Game::Array& arr, int y, int x, MouseEvent event);

	void find_mine();

	void mouse_operate_game(int y, int x, int flag_count, MouseEvent event);

private:

	OperateSet game_set_;
	Game& game_;
};
