#include <set.hpp>
#include <stdio.h>
#include <string>
#include <cassert>

GameSet::GameSet()
{
	bool ans = platform_init();
	assert(ans);
}

GameSet::~GameSet()
{
	platform_restore();
}

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

void GameSet::open_mouse_mode()
{
	printf("\033[?1000h\033[?1003h\033[?1006h\n");
}

void GameSet::close_mouse_mode()
{
	printf("\033[?1000l\033[?1003l\033[?1006l\n");
}

int GameSet::platform_read(char* buf, int len)
{
#ifdef _WIN32
    DWORD read = 0;
    if (!ReadFile(mouse_data_.handle_in, buf, len, &read, nullptr))
    {
        return -1;
    }
    return static_cast<int>(read);
#else
    return static_cast<int>(read(STDIN_FILENO, buf, len));
#endif
}

void GameSet::platform_write(const std::string& str)
{
#ifdef _WIN32
    DWORD written = 0;
    WriteFile(mouse_data_.handle_out, str.data(), static_cast<DWORD>(str.size()), &written, nullptr);
#else
    write(STDOUT_FILENO, str.data(), str.size());
#endif
}

MouseEvent GameSet::get_mouse_event(const std::string& seq)
{
    int cb = 0;
    int cx = 0;
    int cy = 0;
    char term = 0;

    if (parse_sgr_mouse(seq, cb, cx, cy, term) == false)
    {
        return {};
    }
    MouseEvent event = report_mouse(cb, cx, cy, term);
    return event;
}

bool GameSet::parse_sgr_mouse(const std::string& seq, int& cb, int& cx, int& cy, char& term)
{
    // 期望格式：ESC [ < Cb ; Cx ; Cy (M|m)
    if (seq.size() < 6)
    {
        return false;
    }
    if (seq[0] != '\033' || seq[1] != '[' || seq[2] != '<')
    {
        return false;
    }

    term = seq.back();
    if (term != 'M' && term != 'm')
    {
        return false;
    }

    // 去掉 ESC [ < 和结尾的 M/m
    std::string body = seq.substr(3, seq.size() - 4);
    if (sscanf(body.c_str(), "%d;%d;%d", &cb, &cx, &cy) != 3)
    {
        return false;
    }

    return true;
}

MouseEvent GameSet::report_mouse(int cb, int cx, int cy, char term)
{
    MouseEvent event;

    // SGR 坐标从 1 开始，转 0-based
    event.mouse_x = cx - 1;
    event.mouse_y = cy - 1;
    event.operate = MouseOperate::mo_nothing;
    event.action = (term == 'M') ? MouseAction::ma_nothing : MouseAction::ma_release;

    if (cb & 64)
    {
        // 滚轮：64 向上，65 向下，66 向左，67 向右
        int wheel = cb & 3;
        if (wheel == 0) event.operate = MouseOperate::roller_up;
        else if (wheel == 1) event.operate = MouseOperate::roller_down;
        else if (wheel == 2) event.operate = MouseOperate::roller_left;
        else                 event.operate = MouseOperate::roller_right;
        event.action = MouseAction::roll;
    }
    else if (cb & 32)
    {
        // 移动事件
        switch (cb & 3)
        {
        case 0: event.operate = MouseOperate::left_click_and_drag; break;
        case 1: event.operate = MouseOperate::middle_click_and_drag; break;
        case 2: event.operate = MouseOperate::right_click_and_drag; break;
        case 3: event.operate = MouseOperate::mo_nothing; break;
        }
        event.action = MouseAction::move;
    }
    else
    {
        switch (cb & 3)
        {
        case 0: event.operate = MouseOperate::left_click; break;
        case 1: event.operate = MouseOperate::middle_click; break;
        case 2: event.operate = MouseOperate::right_click; break;
        case 3: event.operate = MouseOperate::mo_release; break;
        }
    }

    if (cb & 4)  event.keyboard_char = KeyboardAharacter::shift;
    if (cb & 8)  event.keyboard_char = KeyboardAharacter::alt;
    if (cb & 16) event.keyboard_char = KeyboardAharacter::ctrl;

    return event;
}

bool GameSet::platform_init()
{
#ifdef _WIN32
    mouse_data_.handle_in = GetStdHandle(STD_INPUT_HANDLE);
    mouse_data_.handle_out = GetStdHandle(STD_OUTPUT_HANDLE);

    if (mouse_data_.handle_in == INVALID_HANDLE_VALUE || mouse_data_.handle_out == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    GetConsoleMode(mouse_data_.handle_in, &mouse_data_.old_in_mode);
    GetConsoleMode(mouse_data_.handle_out, &mouse_data_.old_out_mode);

    // 输入：原始模式 + VT 输入
    DWORD in_mode = mouse_data_.old_in_mode;
    in_mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
    in_mode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
    in_mode |= ENABLE_EXTENDED_FLAGS;
    in_mode &= ~ENABLE_QUICK_EDIT_MODE;   // 防止鼠标被控制台截走
    SetConsoleMode(mouse_data_.handle_in, in_mode);

    // 输出：开启 VT 处理，这样鼠标报告序列和颜色才生效
    SetConsoleMode(mouse_data_.handle_out, mouse_data_.old_out_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    return true;
#else
    tcgetattr(STDIN_FILENO, &mouse_data_.old_termios);
    termios t = mouse_data_.old_termios;
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    return true;
#endif
}

void GameSet::platform_restore()
{
#ifdef _WIN32
    if (mouse_data_.handle_in != INVALID_HANDLE_VALUE)
    {
        SetConsoleMode(mouse_data_.handle_in, mouse_data_.old_in_mode);
    }
    if (mouse_data_.handle_out != INVALID_HANDLE_VALUE)
    {
        SetConsoleMode(mouse_data_.handle_out, mouse_data_.old_out_mode);
    }
#else
    tcsetattr(STDIN_FILENO, TCSANOW, &mouse_data_.old_termios);
#endif
}
