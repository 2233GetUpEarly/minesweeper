#include <game/game.hpp>
#include <game/core/operate_set.hpp>
#include <game/show/game_show.hpp>
#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

void test()
{
	OperateSet::hide_cursor();
	srand((unsigned int)time(NULL)); // 使rand函数产生伪随机数
	Game game;
	GameShow game_ui(game);
	game_ui.main_menu();
	OperateSet::display_cursor();
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

