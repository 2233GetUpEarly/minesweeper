#include <set.hpp>
#include <stdio.h>
#include <string>

void GameSet::cursor(int x, int y)
{
	std::string x_str = std::to_string(x);
	std::string y_str = std::to_string(y);
	printf("\033[%s;%sH", x_str.c_str(), y_str.c_str());
}

void GameSet::clear()
{
	printf("\033[2J\033[3J\033[H");
}

void GameSet::hide_cursor()
{
	printf("\033[?25l");
}

void GameSet::display_cursor()
{
	printf("\033[?25h");
}

