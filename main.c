#include "GUI.h" //TODO: performantieproblemen?
#include "logic.h"

int main(){
   initialize_gui();
   while(!initialized_grid && !pressed_quit){
	   draw_window();
	   read_init_input();
   }
   while (!pressed_quit  && game_not_ended()) {
      draw_window();
      read_input();
   }

   free_gui();
   return 0;
}
