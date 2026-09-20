#define _CRT_SECURE_NO_WARNINGS

#include <set.hpp>
#include <string>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

void print_mouse_info(MouseEvent& event)
{
    int x = event.mouse_x;
    int y = event.mouse_y;

    const char* btn = "未知";
    const char* action = (event.action == ma_nothing) ? "按下" : "释放";

    if (event.action == roll) action = "滚动";
    if (event.action == move) action = "移动";

    if (event.operate == roller_up) btn = "滚轮上";
    if (event.operate == roller_down) btn = "滚轮下";
    if (event.operate == roller_left) btn = "滚轮左";
    if (event.operate == roller_right) btn = "滚轮右";

    if (event.operate == left_click_and_drag) btn = "左键拖动";
    if (event.operate == middle_click_and_drag) btn = "中键拖动";
    if (event.operate == right_click_and_drag) btn = "右键拖动";
    if (event.operate == mo_nothing) btn = "无按键移动";

    if (event.operate == left_click) btn = "左键";
    if (event.operate == middle_click) btn = "中键";
    if (event.operate == right_click) btn = "右键";
    if (event.operate == mo_release) btn = "释放";

    printf("坐标(%d,%d) %s %s", x, y, btn, action);

    if (event.keyboard_char == shift)   printf(" +Shift");
    if (event.keyboard_char == alt)     printf(" +Alt");
    if (event.keyboard_char == ctrl)    printf(" +Ctrl");

    printf("\r\n");
    fflush(stdout);
}

void test1();

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    test1();

    return 0;
}

void test1()
{
    GameSet gs;
    gs.open_mouse_mode();
    printf("点击 / 拖动 / 滚轮，q 退出\r\n");

    std::string buf;
    char c;
    MouseEvent event;
    while (true)
    {
        int n = gs.platform_read(&c, 1);
        if (n <= 0) continue;

        if (c == 'q') break;

        buf += c;

        if (!buf.empty() && buf[0] == '\033')
        {
            if (buf.size() >= 3 && buf[0] == '\033' && buf[1] == '[' && buf[2] == '<' &&
                (buf.back() == 'M' || buf.back() == 'm'))
            {
                event = gs.get_mouse_event(buf);
                print_mouse_info(event);
                buf.clear();
            }
            if (buf.size() > 64) buf.clear();
        }
        else
        {
            buf.clear();
        }
    }
    gs.close_mouse_mode();
}
