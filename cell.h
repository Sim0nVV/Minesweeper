#ifndef CELL
#define CELL
//--- Library ---//
#include "stdbool.h"

#define  GRID 		game->grid

struct Cell{
   int mines_nearby;
   bool mine;
   bool visible;
   bool flag;
};

struct Game{
	struct Cell **grid;
	int mines;
	int flags_left;
	int width;
	int height;
};


struct Game *game;

#endif

