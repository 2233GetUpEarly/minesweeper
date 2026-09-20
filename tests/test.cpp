#include "game.hpp"
#include <set.hpp>
#include <game_ui.hpp>
#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

void test()
{
	GameSet::hide_cursor();
	srand((unsigned int)time(NULL)); // 使rand函数产生伪随机数
	Game game;
	GameUI game_ui(game);
	game_ui.main_menu();
	GameSet::display_cursor();
}

int main()
{
#ifdef _WIN32
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
#endif

	test();
	return 0;
}

