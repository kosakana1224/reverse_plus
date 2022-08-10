#ifndef OSEROPLUS_H_
#define OSEROPLUS_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#define WIDTH 9
#define HEIGHT 9
#define BLANK 0
#define BLACK 1
#define WHITE 2
#define RED 3
void menu();
void put_stone(int x,int y,int my_color);
int num_return(int x,int y,int my_color);
void reset_board();
int can_put_count(int my_color);
void display_board();
int max(int a,int b,int c);
int x_convert(int now_x);
int y_convert(int now_y);
int x_convert2(int board_x);
int y_convert2(int board_y);
void cpu_LV1(int cpu_color,int cpu_level);
void cpu_LV2(int cpu_color,int cpu_level);
void game();
void CPU(int cpu_color,int cpu_level);
int total_stone_num(int color);
#endif /* OSEROPLUS_H_ */