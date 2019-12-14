#include "io.h"


void initialize_struct(int w, int h, int m){

	game =(struct Game*) malloc(sizeof(struct Game));
	game->grid = malloc(sizeof(struct Cell*) * w);

	for(int i =0; i<w;i++){
		game->grid[i] = malloc(sizeof(struct Cell)*h);
		for(int j = 0; j<h; j++){
			GRID[i][j].mines_nearby = 0;
			GRID[i][j].mine  = false;
			GRID[i][j].visible  = false;
			GRID[i][j].flag = false;

		}

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
		if ((opt = *++*argv) != '\0'){ // eerst deref (meest links), dan inc, dan deref
			switch (opt){
				case 'w':
					width = atoi(*++argv);
					printf("%i\n", width);
					break;
				case 'h':
					height = atoi(*++argv);
					printf("%i\n", height);
					break;
				case 'm': // Deze case wordt eerste keer gecheckt
					mines = atoi(*++argv);
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


void save_to_file(char *path){
	FILE *file = fopen(path, "a");

	fprintf(file,"//\n");
	fprintf(file, "%i-%i-%i\n", game->width, game->height,game->mines);
	for(int y = 0; y<game->height;y++){
		printf("save to file outer loop %i\n", y);
		for(int x = 0; x<game->width; x++){
			if(GRID[x][y].flag){
				fputc('F',file);
			} else {
				fputc('0',file);
			}
			fputc(return_char_on(x,y,true),file);
			fputc('-',file);
		}
		fputc('\n',file);
	}

	fclose(file);

}
/*
   void read_txt_file( ){

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


