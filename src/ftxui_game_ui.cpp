#include <game/core/game_data.hpp>
#include <game/logic/game_logic.hpp>

#include <game/ui/ftxui_game_ui.hpp>
#include <ftxui/ftxui.hpp>
#include <cassert>
#include <iostream>

using namespace ftxui;

void FtxuiGameUI::ftxui_mouse_operate_game(int y, int x, int flag_count, int is_left)
{
    if (is_left == 1)
    {
        if (game_data_.show[y][x] == '*')                    // 判断输入的坐标是否被占用
        {
            if (game_data_.first_operate)
            {
                game_logic_.set_mine(y, x);
                game_logic_.set_first_operate(0);
            }
            if (game_data_.mine[y][x] == '1')                // 判断输入的坐标是否是雷
            {
                game_logic_.set_win(-9);
            }
            else
            {
                int count = game_logic_.mine_count(y, x);
                game_logic_.set_board_one_char(show_board, y, x, count + '0');
                game_logic_.set_win(game_data_.win + 1);
                game_logic_.spread(y, x);
            }
        }

        if (game_data_.show[y][x] == 'F')
        {
            game_logic_.set_board_one_char(show_board, y, x, '*');
        }

        if (game_data_.show[y][x] >= '1' && game_data_.show[y][x] <= '7')
        {
            game_logic_.number_spread(y, x, game_data_.show[y][x] - '0');
        }
    }

    // 右键加棋子
    if (is_left == 0)
    {
        if (game_data_.show[y][x] == '*' && flag_count < game_data_.mine_count)
        {
            game_logic_.set_board_one_char(show_board, y, x, 'F');
        }
    }
}

Component FtxuiGameUI::get_col_button(int row, int col)
{
    auto col_container = Container::Horizontal({});

    for (int i = 0; i < col; ++i)
    {
        auto option = ButtonOption();
        option.transform = [&, row, i](const EntryState& s)
            {
                // 只返回文本，不调用 border 或任何边框装饰器
                char ch = game_data_.show[row + 1][i + 1];

                auto element = text(char_to_ui_[ch]);
                // 可选：保留焦点高亮，比如加粗或反色
                if (s.focused)
                {
                    element |= bold; // 或者 |= inverted;
                }
                return element;
            };
        option.on_click = [this, row, i]()
            {
                this->ftxui_mouse_operate_game(row + 1, i + 1, 0, 1);
            };

        auto cell = Button(option);
        col_container->Add(cell); // 逐个加入
    }
    return col_container;
}

Component FtxuiGameUI::get_button(int row, int col)
{
    auto row_container = Container::Vertical({});
    for (int i = 0; i < row; ++i)
    {
        auto cols = get_col_button(i, col);
        row_container->Add(cols);
    }

    return row_container;
}

Component FtxuiGameUI::get_mines(int row, int col)
{
    auto mines = get_button(row, col);

    auto component = Renderer(mines, [=] {
        return hbox({ text("") | flex, vbox({ mines->Render() | border, text("") | flex }), text("") | flex});
    });

    return component;
}

// 扫雷难度配置
struct Difficulty
{
    std::string name;
    int rows;
    int cols;
};

FtxuiGameUI::FtxuiGameUI(GameLogic& game_logic, const GameData& game_data)
    :app_(std::make_unique<App>(ScreenInteractive::Fullscreen()))
    , game_data_(game_data)
    , game_logic_(game_logic)
{
    game_logic_.set_first_operate(1);
    game_logic_.set_mine_count(10);
    game_logic_.set_board_area(9, 9);
    game_logic_.init_board(mine_board, '0');
    game_logic_.init_board(show_board, '*');

    char_to_ui_.insert({ '*', "🟪" });
    char_to_ui_.insert({ 'F', "🚩" });
    char_to_ui_.insert({ '0', "  " });
    char_to_ui_.insert({ '1', "① " });
    char_to_ui_.insert({ '2', "② " });
    char_to_ui_.insert({ '3', "③ " });
    char_to_ui_.insert({ '4', "④ " });
    char_to_ui_.insert({ '5', "⑤ " });
    char_to_ui_.insert({ '6', "⑥ " });
    char_to_ui_.insert({ '7', "⑦ " });
    char_to_ui_.insert({ '8', "⑧ " });
}

void FtxuiGameUI::main_menu()
{
    std::vector<Difficulty> difficulties = {
    { "初级", 9, 9 },
    { "中级", 16, 16 },
    { "高级", 16, 30 }
    };
    int selected_difficulty = 0; // 当前选中的索引
    bool auto_flag = false;      // 自动插旗选项状态

    // 顶部控件：难度选择 + 自动插旗
    std::vector<std::string> difficulty_names;
    for (const auto& d : difficulties)
    {
        difficulty_names.push_back(d.name);
    }

    auto radiobox = Radiobox(&difficulty_names, &selected_difficulty);
    auto checkbox = Checkbox("自动插旗", &auto_flag);

    // 底部格子容器（保持实例稳定，动态增删子组件）
    auto grid_container = Container::Vertical({});

    // 记录上一次的难度，用于判断是否需要重建网格
    int last_difficulty = -1;

    // 4. 根组件：组合顶部和底部，并在渲染时动态管理 grid_container
    auto root = Container::Vertical({ Container::Horizontal({ radiobox, checkbox }), grid_container });

    // 用 Renderer 包装 root，以便在每次渲染前根据难度重建网格
    auto renderer = Renderer(root, [&] {
        // 难度发生变化时，清空并重建网格子组件
        if (selected_difficulty != last_difficulty)
        {
            grid_container->DetachAllChildren();

            const auto& diff = difficulties[selected_difficulty];
            grid_container->Add(get_mines(diff.rows, diff.cols));
            last_difficulty = selected_difficulty;
        }

        // 组装最终界面
        auto top = hbox(
            {
                text("难度: ") | bold,
                radiobox->Render(),
                separator(),
                checkbox->Render()
            }) | border;

        auto bottom = grid_container->Render() | border;

        return vbox({ top, bottom });
        });

    app_->Loop(renderer);
}

FtxuiGameUI::~FtxuiGameUI()
{

}
