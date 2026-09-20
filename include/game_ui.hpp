#pragma once

#include <game.hpp>
#include <set.hpp>

class GameUI
{
public:

	GameUI(Game&);

	~GameUI();

	void run();

	void adjust_screen();

	void main_menu();

	int option();

	int display(Game::Array& arr, int y, int x, MouseEvent event);

	void find_mine();

	void mouse_operate_game(int y, int x, int flag_count, MouseEvent event);

private:

	GameSet game_set_;
	Game& game_;
};
