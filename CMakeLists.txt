cmake_minimum_required(VERSION 3.10)

project(minesweeper)

if(MSVC)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>") 
endif()


file(GLOB GAME_SRC_LISTS "src/*.cpp")

add_executable(GameTest "tests/test.cpp" ${GAME_SRC_LISTS})

target_include_directories(GameTest 
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

add_executable(unit_test "tests/unit_test.cpp" "src/set.cpp")
target_include_directories(unit_test 
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)
