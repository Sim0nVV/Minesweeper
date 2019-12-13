#ifndef CELL
#define CELL
//--- Library ---//
#include "stdbool.h"
struct Cell{
   int mines_nearby;
   int mine;
   bool visible;
   bool flag;
};

struct Game;

#endif

