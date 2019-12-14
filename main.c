#include "GUI.h" 
#include "logic.h"
#include "io.h"

int main(int argc, char *argv[]){
	int mines = read_commandline_args(argc,++argv);
	printf("%i, test\n",mines);
	initialize_gui();
	printf("after init gui\n");
	while(!initialized_grid && !pressed_quit){
		draw_window();
		printf("after draw_wnd\n");
		read_init_input(mines);
	}
	while (!pressed_quit  && game_not_ended()) {
		draw_window();
		read_input();
	}

	free_gui();
	//free_cells();;
	return 0;
}
