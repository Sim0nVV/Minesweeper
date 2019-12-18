#include "GUI.h"  //TODO: performantieproblemen (na 30 seconden aanlaten);
#include "logic.h"
#include "io.h"
#include <unistd.h> //for sleep function

int main(int argc, char *argv[]){
	read_commandline_args(argc,++argv);
	if(!pressed_quit){
		initialize_gui();
		draw_window();
		read_input();

		printf("game_not_ended : %i\n", game_not_ended());
		while (!pressed_quit &&game_not_ended()) {
			print_covered_field();
			print_uncovered_field();
			draw_window();
			read_input();
			save_to_file("save.txt");
		}
		print_final_message();
		make_grid_visible();
		draw_window();
		printf("game_not_ended : %i\n", game_not_ended());

		if(!pressed_quit){
			draw_window();
			sleep(3);
		}
		free_gui();
		free_struct();
	}

	return 0;
}
