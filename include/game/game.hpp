#pragma once

#include <game/logic/game_logic.hpp>
#include <game/show/game_show.hpp>

struct MouseEvent;

class Game
{
public:

	Game();

	~Game();

	void run();

private:

	GameData game_data_;
	GameLogic game_logic_;
	GameShow game_show_;
};

