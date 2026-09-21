#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h> // time 函数需要的头文件
#include <stdlib.h> // rand、 srand、malloc 函数需要的头文件
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

struct MouseEvent;

struct GameData
{
	std::vector<std::vector<char>> mine;
	std::vector<std::vector<char>> show;
	int rows;
	int cols;
	int mine_count;
	int row;
	int col;
	int win;
	int begin_time;
	int first_operate;
};

class Game
{
public:

	using Array = std::vector<std::vector<char>>;

	Game();

	~Game();

	GameData& get_game_data();

	void get_mine(int num);

	void init_board(Game::Array& arr, char set);

	void set_mine(int y, int x);

	int mine_count(int y, int x);

	void spread(int y, int x);

	void number_spread(int y, int x, int mine_number);

private:

private:

	GameData data_;
};

