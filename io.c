#include "cell.h"
#include <stdio.h>
#include <stdlib.h>

void initialize_struct(int w, int h, int m){


	game =(struct Game*) malloc(sizeof(struct Game));
	game->grid = malloc(sizeof(struct Cell*) * w);

	for(int i =0; i<w;i++){
		game->grid[i] = malloc(sizeof(struct Cell)*h);
	}


	printf("W: %i, H:%i, m:%i\n", w,h,m);

	game->mines = m;
	game->flags_left = m; // er moeten evenveel mijnen als vlaggen zijn
	game->width = w;
	game->height = h;
}


int  read_commandline_args(int argc, char *argv[]){
	int opt;
	int width, height, mines;



	while(--argc > 0 && **argv =='-'){ //eerst increment, dan dereference
		while ((opt = *++*argv) != '\0'){ // eerst deref (meest links), dan inc, dan deref
			switch (opt){
				case 'w':
					width = **++argv - '0';
					printf("%i\n", width);
					break;
				case 'h':
					height = **++argv - '0';
					printf("%i\n", height);
					break;
				case 'm': // Deze case wordt eerste keer gecheckt
					mines = **++argv - '0';
					printf("%i\n", mines);
					break;
				default:
					printf("illegal option, %c\n",opt);
					break;

			}

		}
		**++argv;
		--argc;

	}

	// branch naar init struct
	initialize_struct(width,height,mines);

	printf("read commandline\n");

	// branc naar read txt file
	return mines;

}


/*
   void output_curr_iter(){

   }

   void read_txt_file(){

   }
   */

void free_struct(){
	//TODO: Vraag nog eens na of dit correct is
	for(int i =0; i<game->width;i++){
		free(game->grid[i]);
	}
	free(game->grid);
	free(game);


}


