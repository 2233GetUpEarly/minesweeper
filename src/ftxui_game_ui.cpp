#include <game/core/game_data.hpp>
#include <game/logic/game_logic.hpp>

#include <game/ui/ftxui_game_ui.hpp>
#include <ftxui/ftxui.hpp>
#include <cassert>

using namespace ftxui;

Component get_col_button(int col)
{
    auto col_container = Container::Horizontal({});

    auto option = ButtonOption();
    option.transform = [](const EntryState& s)
    {
        // 只返回文本，不调用 border 或任何边框装饰器
        auto element = text(s.label);
        // 可选：保留焦点高亮，比如加粗或反色
        if (s.focused)
        {
            element |= bold; // 或者 |= inverted;
        }
        return element;
    };

    for (int i = 0; i < col; ++i)
    {
        auto cell = Button("🟪", [] { /* 翻开第 i 格 */ }, option);
        col_container->Add(cell); // 逐个加入
    }
    return col_container;
}

Component get_button(int row, int col)
{
    auto row_container = Container::Vertical({});
    for (int i = 0; i < row; ++i)
    {
        auto cols = get_col_button(col);
        row_container->Add(cols);
    }

    return row_container;
}

Component get_mines(int row, int col)
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
    ,game_data_(game_data)
    ,game_logic_(game_logic)
{
    ;
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
            grid_container = get_mines(diff.rows, diff.cols);
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
