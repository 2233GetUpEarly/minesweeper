#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace ftxui
{
	class App;
	class ComponentBase;
	using Component = std::shared_ptr<ComponentBase>;
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

	void ftxui_mouse_operate_game(int y, int x, int flag_count, int is_left);

	ftxui::Component get_col_button(int row, int col);
	ftxui::Component get_button(int row, int col);
	ftxui::Component get_mines(int row, int col);

private:

	std::unique_ptr<ftxui::App> app_;
	std::unordered_map<char, std::string> char_to_ui_;

	const GameData& game_data_;
	GameLogic& game_logic_;
};
