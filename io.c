#include "io.h"
#include "GUI.h"
/*
 * This function allocates the correct amount of memory for the game struct.
 * It also initializes the struct with the correct values (standard values)
 */
void initialize_struct(int w, int h, int m){
	
	game =(struct Game*) malloc(sizeof(struct Game));
	game->grid = malloc(sizeof(struct Cell*) * w);

	for(int i =0; i<w;i++){
		GRID[i] = malloc(sizeof(struct Cell)*h);

		for(int j = 0; j<h; j++){
			GRID[i][j].mines_nearby = 0; //(GRID == game->grid)
			GRID[i][j].mine  = false;
			GRID[i][j].visible  = false;
			GRID[i][j].flag = false;
		}

	}
	game->mines = m;
	game->flags_left = m; // er moeten evenveel mijnen als vlaggen zijn
	game->width = w;
	game->height = h;
}

/*
 * Function that reads multiple characters from file to int
 * for example the string "0230-" will be read as 230
 * for example the string "M-" will be read as 'M'
 */
int read_int_from_file(FILE *file){ 

	int sum;
	int i;
	sum = 0;
	i = fgetc(file);
	if(i == 'M'){
		fgetc(file);
		return 'M';
	}

	while(i != '-' && i != EOF){
		i -= '0';
		sum *= 10;
		sum += i;
		i = fgetc(file);

	}
	return sum;
}
/*
 * function that reads the grid in the save file. 
 * The first value of the grid value is a flag 
 * The second value must be the visibility value (true if visible)
 * The other values up till the '-' will be read as an int
 */
void init_game_from_file(FILE *file, int width, int height){
	char flag_in_file, second_value;
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			flag_in_file = fgetc(file);

				if(flag_in_file - '0'){
					GRID[x][y].flag = true; // maximum 8 mines. This is encoded
				}

				if(fgetc(file) - '0'){
					GRID[x][y].visible = true;
				}

				switch(second_value = read_int_from_file(file)){
					case 'M':
						GRID[x][y].mine = true;
						break;
					default:
						GRID[x][y].mines_nearby = second_value;
						break;
				}

		}
		fgetc(file); //read the newline
	}
}
/*
 * function that reads the txt_file
 * First checks whether it is a correct file
 * Then reads game state (width, height, mines, flags_left), and then delegates to other functions
 */
void read_txt_file(char *path ){
	FILE *file = fopen(path, "r");

	int width,height, mines,flags_left;

	int first_line_length = 3;
	char line[first_line_length];
	if(fgets(line, first_line_length, file) != NULL){
		fgetc(file);
		if((strcmp(line,"//") == 0)){ //if file starts with this, it is valid
			width = read_int_from_file(file);
			height = read_int_from_file(file);
			mines = read_int_from_file(file);
			flags_left = read_int_from_file(file);
			fgetc(file);
			initialize_struct(width,height,mines);
			game->flags_left = flags_left;
			init_game_from_file(file,width,height);

		} else 
			printf("format corrupted\n");
		
		initialized_grid = true;
		fclose(file);
	}


}
/*
 * Function that can read 2 types of command line arguments
 * Can take a width, height and mine value to initialise the game
 * OR can take a -f argument that specifies the location of a file
 */
void read_commandline_args(int argc, char *argv[]){
	int opt;
	int width, height, mines;
	char * file;
	bool read_file;

	while(--argc > 0 && **argv =='-'){ //eerst increment, dan dereference
		if ((opt = *++*argv) != '\0'){ // eerst deref (meest links), dan inc, dan deref
			switch (opt){
				case 'w':
					width = atoi(*++argv);
					break;
				case 'h':
					height = atoi(*++argv);
					break;
				case 'm': // Deze case wordt eerste keer gecheckt
					mines = atoi(*++argv);
					break;
				case 'f':
					file = (*++argv);
					read_txt_file(file);
					read_file = true;
					break;
				default:
					printf("illegal option, %c\n",opt);
					break;
			}

		}
		++argv;
		--argc;

	}

	if(!read_file){
		if(width*height>mines)
			initialize_struct(width,height,mines);
		else {
			printf("There are too much mines for the board size. Try again!\n");
			pressed_quit = true;
		}
	}

}

/*
 * outputs to save file
 * First two backspaces to indicate the start
 * then 4 values (width,height, mines and flags_left)
 * then each cell with 
 * first value -> if there's a flag on it
 * second value -> if cell is visible
 * other values -> an integer (that indicates nearby mines) or a mine
 */
void save_to_file(char *path){
	FILE *file = fopen(path, "w");

	fprintf(file,"//\n");
	fprintf(file, "%i-%i-%i-%i-\n", game->width, game->height,game->mines,game->flags_left);
	for(int y = 0; y<game->height;y++){
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
			} else {
				fputc(GRID[x][y].mines_nearby + '0',file);
			}

			fputc('-',file);
		}
		fputc('\n',file);
	}

	fclose(file);

}



/*
 * Frees all dynamically allocated memory by the struct
 */
void free_struct(){
	for(int i =0; i<game->width;i++){
		free(game->grid[i]);
	}
	free(game->grid);
	free(game);


}

