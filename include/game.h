#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h> // time 函数需要的头文件
#include <stdlib.h> // rand、 srand、malloc 函数需要的头文件 

void initBoard(char** arr, int rows, int cols, char set);

void display(char** arr, int row, int col);

void setMine(char** mine, int row, int col, int y, int x);

void findMine(char** mine, char** show, int row, int col);

void spread(char** mine, char** show, int y, int x, int* win, int row, int col);

void numberSpread(char** mine, char** show, int y, int x, int num, int* win, int row, int col);

// 动态扫雷实现
char** apply(int* rows, int* cols);

void release(char** arr, int rows, int cols);

int getMine();

void myApply(int* rows, int* cols);

int myGetMine();