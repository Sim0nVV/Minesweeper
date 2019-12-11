#include "GUI.h" //TODO: performantieproblemen?
#include "logic.h"

int main(int argc, char *argv[]){
	int opt;
	while (--argc >0 && **++argv =='-')
		while ((opt = *++*argv) != '\0')
			switch (opt) {
				case 'x':
					except = 1;
					break;
				case 'n':
					number = 1;
					break;
				default:
					printf("find: illegal option %c\n", opt);
					argc = 0;
					found = -1;
					break;
			}
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
