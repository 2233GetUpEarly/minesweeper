#include <game/ui/ftxui_game_ui.hpp>
#include <game/game.hpp>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <signal.h>
#endif

void ftxui_test()
{
	GameData game_data;
	GameLogic game_logic(game_data);

	FtxuiGameUI fgame(game_logic, game_data);
	fgame.main_menu();

	srand((unsigned int)time(NULL)); // 使rand函数产生伪随机数
	//Game game;
	//game.run();
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

	ftxui_test();
	return 0;
}
