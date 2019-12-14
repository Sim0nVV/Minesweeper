#ifndef MACROS_H
#define MACROS_H
#include <stdbool.h>
#include <time.h>


extern void read_commands();
extern char return_char_on(int x, int y, bool visible);

void toggle_flag(int x,int y);
bool game_not_ended();
void reveal(int x, int y);
void initialize_grid(int n, int init_x, int init_y);


#endif
