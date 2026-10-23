#pragma once

#include <memory>

namespace ftxui
{
	class App;
}

class GameLogic;
struct GameData;

class FtxuiGameUI
{
public:

	FtxuiGameUI(GameLogic&, const GameData&);
	~FtxuiGameUI();

	void main_menu();

private:

	std::unique_ptr<ftxui::App> app_;

	const GameData& game_data_;
	GameLogic& game_logic_;
};
