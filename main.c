#include "GUI.h"

int main()
{
   bool should_continue = true;
   initialize_gui();
   while (should_continue) {
      draw_window();
   }
   //read_commands();
   return 0;
}
