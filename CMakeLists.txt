cmake_minimum_required(VERSION 3.10)

project(minesweeper)

if(MSVC)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>") 
endif()

find_package(ftxui CONFIG REQUIRED)

add_library(game_basic STATIC "src/game.cpp" "src/game_logic.cpp")
target_include_directories(game_basic PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(native_ui STATIC "src/game_ui.cpp" "src/game_show.cpp" "src/operate_set.cpp")
target_include_directories(native_ui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_library(ftxui_ui STATIC "src/ftxui_game_ui.cpp")
target_include_directories(ftxui_ui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(ftxui_ui PRIVATE ftxui::ftxui)

add_executable(game_use_ftxui "tests/ftxui_test.cpp")
target_link_libraries(game_use_ftxui PRIVATE game_basic ftxui_ui)

add_executable(GameTest "tests/test.cpp")
target_link_libraries(GameTest PRIVATE game_basic native_ui)

add_executable(unit_test "tests/unit_test.cpp" "src/operate_set.cpp")
target_include_directories(unit_test 
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)
