#include <game/game.hpp>

Game::Game()
	:game_logic_(game_data_)
	,game_show_(game_logic_, game_data_)
{
	;
}

Game::~Game()
{

}

void Game::run()
{
	game_show_.main_menu();
}
