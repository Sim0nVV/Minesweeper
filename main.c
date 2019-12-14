#include "GUI.h"  //TODO: performantieproblemen (na 30 seconden aanlaten);
#include "logic.h"
#include "io.h"
#include <unistd.h> //for sleep function

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
	save_to_file("test.txt");
	while (!pressed_quit &&game_not_ended()) {
	print_covered_field();
		printf("game_not_ended happen\n");
		draw_window();
		read_input();
		save_to_file("test.txt");
	}
	print_final_message();
	make_grid_visible();
	draw_window();

	if(!pressed_quit){
		draw_window();
		sleep(3);
	}
	

	free_gui();
	free_struct();
	return 0;
}
