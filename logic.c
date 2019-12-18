#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "logic.h"
#include "cell.h"


#define BOUNDARIES(i,j) 	if (i < 0 || i >= game->width || j < 0 || j >= game->height) { continue;}
#define INPUT_TO_INT(i)		input[i] - 48
#define SINGLE_DIGIT(i) 	(0 <= i && i <= 9)

int flags_planted; 
bool dead = false;

// Datatype die bevat wat er op een bepaalde coordinaat is
struct Cell;

//struct Cell grid[game->width][game->height] ; //multidimensionale array van structs
/*
   struct Game{
   struct Cell grid[game->width][game->height];
   int width;
   int height;
   int mines;
   };
   */

char return_char_on(int x, int y, bool visible){ // Op basis van x en y-coordinaat geeft deze de correcte char terug
	if (GRID[x][y].flag){ 	// Boolean visible dient voor te printen met P commando (dus 'debug' mode)
		return 'F';
	} else if(visible || GRID[x][y].visible){
		if (GRID[x][y].mine){
			return 'M';
		} else {
			return GRID[x][y].mines_nearby + 48; //conversion naar char
		}
	}else { 
		return ' ';
	}
}

void print_first_line(){ // functie print de eerste lijn van het bord (met x-coordinaten dus)
	printf("   ");
	for(int i = 0; i<game->width; i++){
		printf(" %i  ", i);
	}
	printf("\n");
}

void print_next_lines(bool visible){ // print het bord zelf. krijgt een boolean binnen die moet tekenen als het P commando gebeurde
	for(int y = 0; y<game->height; y++){
		printf("%i ", y);
		for(int x = 0; x<game->width; x++){
			printf("| %c|", return_char_on(x,y,visible)); 
		}	
		printf("\n");
	}
}

void print_covered_field(){ // print het veld in normale speelmodus
	printf("\nRemaining flags: %i\n\n", game->flags_left);
	print_first_line();
	print_next_lines(false); // boolean geeft aan of het hele veld getoond moet worden
}

void print_uncovered_field(){ // print het veld in 'debug mode'
	print_first_line();
	print_next_lines(true); // boolean geeft aan of hele veld geprint moet worden
}

void toggle_flag(int x,int y){ // zet een vlag aan op x- en y-coordinaat
	if(game->flags_left <= 0){
		printf("Maximum amount of planted flags reached.\n");
		if(GRID[x][y].flag){
			GRID[x][y].flag = !GRID[x][y].flag;
			game->flags_left += 1;
		} 
	} else if(GRID[x][y].flag) {
		GRID[x][y].flag = !GRID[x][y].flag;
		game->flags_left += 1;
	} else {
		GRID[x][y].flag = !GRID[x][y].flag;
		game->flags_left -= 1;
	}
}

void reveal(int x, int y){ // reveal functie 
	if(!GRID[x][y].flag){ // kan alleen maar reveaelen op plaats zonder vlag
		if(GRID[x][y].mines_nearby > 0 && !GRID[x][y].mine){
			GRID[x][y].visible = true;
		} else if (GRID[x][y].mine){
			dead = true;
		} else {
			for(int i = x - 1; i < x + 2; i++){ // hij gaat erover. Mag niet over grenzen gaan
				for(int j = y - 1; j< y + 2; j++){
					BOUNDARIES(i,j) //test als algoritme niet buiten grenzen spelbord gaat
						if(GRID[i][j].visible || GRID[i][j].mine){
							continue;
						} else if (GRID[i][j].mines_nearby > 0) {
							GRID[i][j].visible = true;
						} else {
							GRID[i][j].visible = true;
							reveal(i,j);
						}
				}
			}

		}
	}
}


void increment_nearby_cells(int x, int y){ //Telkens een mijn toegevoegd wordt. incrementeer buren
	for(int i = x - 1; i < x + 2; i++){ 
		for(int j = y - 1; j< y + 2; j++){
			BOUNDARIES(i,j)
				if(GRID[i][j].mine){
					continue;
				} else {
					GRID[i][j].mines_nearby += 1;
				}
		}
	}
}
bool all_mines_covered(){ // Check als alle mijnen met vlaggen bedekt zijn
	bool check = true;
	for(int x = 0; x< game->width; x++){
		for(int y = 0; y<game->height; y++){
			if (GRID[x][y].mine){
				check = check && GRID[x][y].flag;
			}
		}
	}
	printf("all mines covered happen\n");
	return check;
}

bool all_non_mines_shown(){ // Check of alle niet-mijnen zichtbaar zijn.
	bool check = true;
	for(int x = 0; x< game->width; x++){
		for(int y = 0; y<game->height; y++){
			if (!GRID[x][y].mine){
				check = check && GRID[x][y].visible;
			}
		}
	}
	printf("all_non_mines_shown happen\n");
	return check;
}

void initialize_grid(int mines, int init_x, int init_y){ //initializeer GRID, gebeurt na eerste reveal
	srand(time(NULL)); //geef seed aan random generator

	for(int w = 0; w< game->width; w++){
		for(int h = 0; h< game->height; h++){

		}
	}
	printf("after init struct: line 190 logic.c\n");

	for(int mines_left = mines; mines_left > 0; mines_left--){
		printf("mines_left: %i\n", mines_left);
		int x = rand() % game->width; // code die random vakje zoekt, waar nog geen mijn is
		int y = rand() % game->height; 

		while(GRID[x][y].mine || (init_x == x && init_y == y)){ // op die plaats mag er al geen mijn zijn en moet geldige plaats zijn
			x = rand() % game->width; 
			y = rand() % game->height; 
		} 
		GRID[x][y].mine = true;
		increment_nearby_cells(x, y);


	}
	reveal(init_x,init_y);
}
bool game_not_ended(){
	return !dead && !all_mines_covered() && !all_non_mines_shown(); 
}
void print_final_message(){
	if (dead){
		printf("You Pressed On A Mine. GAME OVER\n\n");
		print_uncovered_field();
	} else if (all_mines_covered()){
		printf("All Mines Covered With Flags. You Won!\n");
	} else if (all_non_mines_shown()){
		printf("All non mines shown. You Won!\n");
	}
}

void make_grid_visible(){
	for(int w = 0; w < game->width;w++){
		for(int h = 0; h< game->height; h++){
			GRID[w][h].visible = true;
		}
	}
}























