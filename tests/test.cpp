#include <game/game.hpp>
#include <game/core/operate_set.hpp>
#include <game/ui/game_ui.hpp>
#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <signal.h>
#endif

void test()
{
	OperateSet::hide_cursor();
	srand((unsigned int)time(NULL)); // 使rand函数产生伪随机数
	Game game;
	game.run();
	OperateSet::display_cursor();
}

int main()
{
#ifdef _WIN32
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);

	SetConsoleCtrlHandler(NULL, TRUE);
#elif __linux__
	signal(SIGINT, SIG_IGN);
#endif

	test();
	return 0;
}

