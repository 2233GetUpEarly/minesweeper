#pragma once

#include <game/core/game_data.hpp>
#include <game/core/operate_set.hpp>

#include <chrono>

class GameUI;

struct MouseOperateFunc
{
	void operator()(int y, int x, int flag_count, MouseEvent event);

	GameUI* ui;
};

class GameShow
{
public:

	GameShow(MouseOperateFunc);

	void main_menu_show(int y, int x);

	void option_menu_show(int y, int x);

	void display(const Array& arr, int y, int x, int win, int begin_time, int row, int col, int mine_count, MouseEvent event);

	int game_over_show(int y, int x, int row, int col, std::chrono::steady_clock::time_point last_click, MouseEvent event);

	int game_win_show(int y, int x, int row, int col, std::chrono::steady_clock::time_point last_click, MouseEvent event);

	int game_back_show(int y, int x, int row, int col, std::chrono::steady_clock::time_point last_click, MouseEvent event);

	void adjust_screen_show();

private:

	MouseOperateFunc mouse_operate_func_;
};
