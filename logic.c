#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "logic.h"
#include "cell.h"


#define BOUNDARIES(i,j) 	if (i < 0 || i >= game->width || j < 0 || j >= game->height) { continue;}
#define INPUT_TO_INT(i)		input[i] - 48
#define SINGLE_DIGIT(i) 	(0 <= i && i <= 9)

bool dead = false;

// Datatype die bevat wat er op een bepaalde coordinaat is
struct Cell;

char return_char_on(int x, int y, bool visible){ // Op basis van x en y-coordinaat geeft deze de correcte char terug
	if (GRID[x][y].flag){ 	// Boolean visible dient voor te printen met P commando (dus 'debug' mode)
		return 'F';
	} else if(visible || GRID[x][y].visible){
		if (GRID[x][y].flag){
			return 'F';
		} else if (GRID[x][y].mine){
			return 'M';
		} else {
			return GRID[x][y].mines_nearby + 48; //conversion naar char
		}
	} else { 
		return ' ';
	}
}
/*
 * Toggles the flag on given x and y coordinates
 */

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
	if(!GRID[x][y].flag){ // kan alleen maar revealen op plaats zonder vlag
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
/*
 * Algorithm to increase nearby neighbour count
 * Selects all neighbours and goes increments them
 */
void increment_nearby_cells(int x, int y){ 
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
/*
 * Check for letting the game end
 */
bool all_mines_covered(){  
	bool check = true;
	for(int x = 0; x< game->width; x++){
		for(int y = 0; y<game->height; y++){
			if (GRID[x][y].mine){
				check = check && GRID[x][y].flag;
			}
		}
	}
	return check;
}
/*
 * Endgame check.
 */
bool all_non_mines_shown(){ 
	bool check = true;
	for(int x = 0; x< game->width; x++){
		for(int y = 0; y<game->height; y++){
			if (!GRID[x][y].mine){
				check = check && GRID[x][y].visible;
			}
		}
	}
	return check;
}
/*
 * Initializes grid. Adds mines only if there hasn't been added one before. 
 */

void initialize_grid(int mines, int init_x, int init_y){ //initializeer GRID, gebeurt na eerste reveal
	srand(time(NULL)); //geef seed aan random generator

	for(int mines_left = mines; mines_left > 0; mines_left--){
		int x = rand() % game->width; 
		int y = rand() % game->height; 

		while(GRID[x][y].mine || (init_x == x && init_y == y)){ 
			x = rand() % game->width; 
			y = rand() % game->height; 
		} 
		GRID[x][y].mine = true;
		increment_nearby_cells(x, y);


	}
	reveal(init_x,init_y);
}
/*
 * Function included in main to check whether game is over or not
 */
bool game_not_ended(){
	return !dead && !all_mines_covered() && !all_non_mines_shown(); 
}
void print_final_message(){
	if (dead){
		printf("You Pressed On A Mine. GAME OVER\n\n");
	} else if (all_mines_covered()){
		printf("All Mines Covered With Flags. You Won!\n");
	} else if (all_non_mines_shown()){
		printf("All non mines shown. You Won!\n");
	}
}

/*
 * Just before ending, this shows the whole grid
 */
void make_grid_visible(){
	for(int w = 0; w < game->width;w++){
		for(int h = 0; h< game->height; h++){
			GRID[w][h].visible = true;
		}
	}
}























