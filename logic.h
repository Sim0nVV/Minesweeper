#ifndef MACROS_H
#define MACROS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "logic.h"
#include "cell.h"


extern void read_commands();
extern char return_char_on(int x, int y, bool visible);

void toggle_flag(int x,int y);
bool game_not_ended();
void reveal(int x, int y);
void initialize_grid(int n, int init_x, int init_y);

void print_final_message();
void make_grid_visible();
// debug stuff
void print_uncovered_field();
void print_covered_field();
void debug_field();


#endif
