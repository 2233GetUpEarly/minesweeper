#pragma once

#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

enum MouseOperate
{
	mo_nothing,
	mo_release,

	roller_up,
	roller_down,
	roller_left,
	roller_right,

	left_click,
	middle_click,
	right_click,

	left_click_and_drag,
	middle_click_and_drag,
	right_click_and_drag,
};

enum MouseAction
{
	ma_nothing,
	ma_release,
	roll,
	move,
};

enum KeyboardAharacter
{
	ka_nothing,
	shift,
	alt,
	ctrl,
};

struct MouseEvent
{
	MouseOperate operate = mo_nothing;
	MouseAction action = ma_nothing;
	KeyboardAharacter keyboard_char = ka_nothing;
	int mouse_x = -1;
	int mouse_y = -1;
};

class GameSet
{
public:

	GameSet();

	~GameSet();

	static void cursor(int x, int y);

	static void clear();

	static void hide_cursor();

	static void display_cursor();

	static void open_mouse_mode();

	static void close_mouse_mode();

	int platform_read(char* buf, int len);

	void platform_write(const std::string& str);

	MouseEvent get_mouse_event(const std::string& seq);

private:

	bool parse_sgr_mouse(const std::string& seq, int& cb, int& cx, int& cy, char& term);

	MouseEvent report_mouse(int cb, int cx, int cy, char term);

	bool platform_init();

	void platform_restore();

private:

	struct MouseData
	{
#ifdef _WIN32
		HANDLE handle_in = INVALID_HANDLE_VALUE;
		HANDLE handle_out = INVALID_HANDLE_VALUE;
		DWORD old_in_mode = 0;
		DWORD old_out_mode = 0;
#else
		termios old_termios;
#endif
	};

	MouseData mouse_data_;
};

