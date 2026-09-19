cmake_minimum_required(VERSION 3.10)

project(minesweeper)

file(GLOB GAME_SRC_LISTS "src/*.c")

add_executable(GameTest "tests/test.c" ${GAME_SRC_LISTS})

target_include_directories(GameTest 
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)
