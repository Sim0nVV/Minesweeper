#include <stdbool.h>
#ifndef MACROS_H
#define MACROS_H

#define  MINES  		5
#define  X_CELLS  		6
#define  Y_CELLS 		6

struct Cell{
   int mines_nearby;
   int mine;
   bool visible;
   bool flag;
};

extern struct Cell grid[X_CELLS][Y_CELLS];





#endif
