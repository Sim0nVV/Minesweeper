#include "GUI.h"
#include "logic.h"

int main()
{
   bool should_continue = true;
   initialize_gui();
   while (should_continue) {
      draw_window();
      read_input();
   }
   free_gui();
   return 0;
}
