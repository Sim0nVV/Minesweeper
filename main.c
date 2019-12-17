#include "GUI.h"  //TODO: performantieproblemen (na 30 seconden aanlaten);
#include "logic.h"
#include "io.h"
#include <unistd.h> //for sleep function

int main(int argc, char *argv[]){
	read_commandline_args(argc,++argv);
	if(!pressed_quit){
		initialize_gui();

		while(!initialized_grid && !pressed_quit){
			draw_window();
			read_input();
		}
		save_to_file("save.txt");
		while (!pressed_quit &&game_not_ended()) {
			print_covered_field();
			draw_window();
			read_input();
			save_to_file("save.txt");
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
	}

	return 0;
}
