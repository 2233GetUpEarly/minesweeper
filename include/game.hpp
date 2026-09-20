#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h> // time 函数需要的头文件
#include <stdlib.h> // rand、 srand、malloc 函数需要的头文件

#ifdef _WIN32
#include <windows.h>
#endif

struct MouseEvent;

void initBoard(char** arr, int rows, int cols, char set);

int display(char** arr, int row, int col, int y, int x, int* time);

void setMine(char** mine, int row, int col, int y, int x);

void findMine(char** mine, char** show, int row, int col);

void spread(char** mine, char** show, int y, int x, int* win, int row, int col);

void numberSpread(char** mine, char** show, int y, int x, int num, int* win, int row, int col);

// 动态扫雷实现
char** apply(int* rows, int* cols);

void game_release(char** arr, int rows, int cols);

int getMine(int num);

// 伪图形扫雷实现
void game();

void gotoxy(int a, int b);

void HideCursor();

int option(int* rows, int* cols);

// 鼠标操作
void getPower();

void mouseOperateGame(char** mine, char** show, MouseEvent event, int y, int x, int row, int col, int* first, int* win, int FCount);

