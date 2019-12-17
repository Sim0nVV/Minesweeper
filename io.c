#include "io.h"
#include "GUI.h"


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

/*
 * Function that reads multiple characters from file to int
 */
int read_int_from_file(FILE *file){ 

	int sum;
	int i;
	sum = 0;
	i = fgetc(file);
	if(i == 'M'){
		return i;
	}

	while(i != '-' && i != EOF){
		printf("sum: %i, i: %c\n", sum, i);
		i -= '0';
		sum *= 10;
		sum += i;
		i = fgetc(file);

	}
	printf("after while\n");
	return sum;
}

void init_game_from_file(FILE *file, int width, int height){
	char flag_in_file, second_value,visible;
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			flag_in_file = fgetc(file);
			while (flag_in_file != '\n' && flag_in_file != EOF){

				printf("flag in file = %c\n", flag_in_file);
				if(flag_in_file - '0'){
					GRID[x][y].flag = true; // maximum 8 mines. This is encoded
				}

				if(fgetc(file) - '0'){
					GRID[x][y].visible = true;
				}

				switch(second_value =  read_int_from_file(file)){
					case 'M':
						GRID[x][y].mine = true;
						break;
					default:
						second_value -= '0';
						GRID[x][y].mines_nearby = second_value;
						break;
				}
				flag_in_file = fgetc(file);
			}

		}
	}
}

void read_txt_file(char *path ){
	FILE *file = fopen(path, "r");
	printf("read_txt_file happens\n");

	int width,height, mines;

	int first_line_length = 3;
	char line[first_line_length];
	if(fgets(line, first_line_length, file) != NULL){
		printf("first line = %s\n", line);
		printf("%i\n",  strcmp(line,"//"));
		fgetc(file);
		if((strcmp(line,"//") == 0)){ //if file starts with this, it is valid
			width = read_int_from_file(file);
			height = read_int_from_file(file);
			mines = read_int_from_file(file);
			initialize_struct(width,height,mines);
			init_game_from_file(file,width,height);

		} else {
			printf("format corrupted\n");
		}
		fclose(file);
	}


}

int  read_commandline_args(int argc, char *argv[]){
	int opt;
	int width, height, mines;
	char * file;


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
				case 'f':
					file = (*++argv);
					printf("%s\n", file);
					read_txt_file(file);
					break;
				default:
					printf("illegal option, %c\n",opt);
					break;

			}

		}
		++argv;
		--argc;

	}

	// branch naar init struct
	if(width*height>mines){
		initialize_struct(width,height,mines);
	} else {
		printf("There are too much mines for the board size. Try again!\n");
		pressed_quit = true;
	}
	// branc naar read txt file
	return mines;

}


void save_to_file(char *path){
	FILE *file = fopen(path, "w");

	fprintf(file,"//\n");
	fprintf(file, "%i-%i-%i-\n", game->width, game->height,game->mines);
	for(int y = 0; y<game->height;y++){
		printf("save to file outer loop %i\n", y);
		for(int x = 0; x<game->width; x++){
			if(GRID[x][y].flag){
				fputc('1',file);
			} else {
				fputc('0',file);
			}

			if(GRID[x][y].visible){
				fputc('1',file);
			} else {
				fputc('0',file);
			}

			if(GRID[x][y].mine){
				fputc('M',file);
			} else{
				fputc(GRID[x][y].mines_nearby + '0',file);
			}

			fputc('-',file);
		}
		fputc('\n',file);
	}

	fclose(file);

}




void free_struct(){
	//TODO: Vraag nog eens na of dit correct is
	for(int i =0; i<game->width;i++){
		free(game->grid[i]);
	}
	free(game->grid);
	free(game);


}

