#pragma once

#include <game/logic/game_logic.hpp>
#include <game/ui/game_ui.hpp>

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
	GameUI game_show_;
};

