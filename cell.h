#ifndef CELL
#define CELL
struct Cell{
   int mines_nearby;
   int mine;
   bool visible;
   bool flag;
};

//extern struct Cell grid[X_CELLS][Y_CELLS];

struct Game;

#endif

