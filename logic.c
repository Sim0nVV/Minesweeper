#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "logic.h"
#include "cell.h"


#define BOUNDARIES(i,j) 	if (i < 0 || i >= X_CELLS || j < 0 || j >= Y_CELLS) { continue;}
#define INPUT_TO_INT(i)		input[i] - 48
#define SINGLE_DIGIT(i) 	(0 <= i && i <= X_CELLS)

int flags_planted; 
bool dead = false;

// Datatype die bevat wat er op een bepaalde coordinaat is
struct Cell;

//struct Cell grid[X_CELLS][Y_CELLS] ; //multidimensionale array van structs

struct Game{
	struct Cell grid[X_CELLS][Y_CELLS];
	int width;
	int height;
	int mines;
};

struct Game game;

char return_char_on(int x, int y, bool visible){ // Op basis van x en y-coordinaat geeft deze de correcte char terug
	if (GRID[x][y].flag){ 			// Boolean visible dient voor te printen met P commando (dus 'debug' mode)
		return 'F';
	} else if(visible || GRID[x][y].visible){
		if (GRID[x][y].flag){
			return 'F';
		} else if (GRID[x][y].mine){
			return 'M';
		} else {
			return GRID[x][y].mines_nearby + 48; //conversion naar char
		}}else { 
			return ' ';
		}
}

void print_first_line(){ // functie print de eerste lijn van het bord (met x-coordinaten dus)
	printf("   ");
	for(int i = 0; i<X_CELLS; i++){
		printf(" %i  ", i);
	}
	printf("\n");
}

void print_next_lines(bool visible){ // print het bord zelf. krijgt een boolean binnen die moet tekenen als het P commando gebeurde
	for(int y = 0; y<Y_CELLS; y++){
		printf("%i ", y);
		for(int x = 0; x<X_CELLS; x++){
			printf("| %c|", return_char_on(x,y,visible)); 
		}	
		printf("\n");
	}
}

void print_covered_field(){ // print het veld in normale speelmodus
	printf("\nRemaining flags: %i\n\n", flags_planted);
	print_first_line();
	print_next_lines(false); // boolean geeft aan of het hele veld getoond moet worden
}

void print_uncovered_field(){ // print het veld in 'debug mode'
	print_first_line();
	print_next_lines(true); // boolean geeft aan of hele veld geprint moet worden
}

void toggle_flag(int x,int y){ // zet een vlag aan op x- en y-coordinaat
	if(flags_planted == 0){
		printf("Maximum amount of planted flags reached.\n");
		if(GRID[x][y].flag){
			GRID[x][y].flag = !GRID[x][y].flag;
			flags_planted += 1;
		} else {
			GRID[x][y].flag = !GRID[x][y].flag;
			flags_planted -= 1;
		}
	} else if(GRID[x][y].flag) {
		GRID[x][y].flag = !GRID[x][y].flag;
		flags_planted += 1;
	} else {
		GRID[x][y].flag = !GRID[x][y].flag;
		flags_planted -= 1;
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


void process_command(char *input){ //input parser
	if(!((SINGLE_DIGIT(input[2])) && SINGLE_DIGIT(input[4]))){ // check zodat getallen tussen 0 en 9 zijn
		char command = input[0]; // probleem met uitlezen string
		switch (command){
			case 'P':
				print_uncovered_field();
				break;
			case 'F':
				toggle_flag(INPUT_TO_INT(2), INPUT_TO_INT(4)); //We geven geen getallen in groter dan 9
				print_covered_field();
				break;
			case 'R':
				reveal(INPUT_TO_INT(2),INPUT_TO_INT(4));
				print_covered_field();
				break;
			default:
				printf("Not a known commando\n");

		}
	} else {
		printf("You haven't entered digits between 0 and 9");
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
	for(int x = 0; x< X_CELLS; x++){
		for(int y = 0; y<Y_CELLS; y++){
			if (GRID[x][y].mine){
				check = check && GRID[x][y].flag;
			}
		}
	}
	return check;
}

bool all_non_mines_shown(){ // Check of alle niet-mijnen zichtbaar zijn.
	bool check = true;
	for(int x = 0; x< X_CELLS; x++){
		for(int y = 0; y<Y_CELLS; y++){
			if (!GRID[x][y].mine){
				check = check && GRID[x][y].visible;
			}
		}
	}
	return check;
}

void initialize_grid(int mines, int init_x, int init_y){ //initializeer GRID, gebeurt na eerste reveal
	flags_planted = mines;

	for(int mines_left = mines; mines_left >= 0; mines_left--){
		printf("mines_left: %i\n", mines_left);
		int x = rand() % X_CELLS; // code die random vakje zoekt, waar nog geen mijn is
		int y = rand() % Y_CELLS; 

		while(GRID[x][y].mine || (init_x == x && init_y == y)){ // op die plaats mag er al geen mijn zijn en moet geldige plaats zijn
			x = rand() % X_CELLS; 
			y = rand() % Y_CELLS; 
		} 
		GRID[x][y].mine = true;
		increment_nearby_cells(x, y);

		reveal(init_x,init_y);

	}
}
bool game_not_ended(){
	return !dead && !all_mines_covered() && !all_non_mines_shown(); 
}

void read_commands(){
	srand(time(NULL)); //geef seed aan random generator
	char input[6];
	printf("Insert a Command: R for Reveal, F for Flag, P for show uncovered field\n");
	scanf("%[^\n]%*c",input); 
	while(input[0] != 'R' || ((SINGLE_DIGIT(input[2])) && SINGLE_DIGIT(input[4]))){ //zolang reveal command niet gegeven wordt. initializeer nog niet
		process_command(input);
		scanf("%[^\n]%*c",input); 
	}
	//initialize_grid(MINES, 1, 1);
	do { // doe minstens 1 keer het scanf commmando.
		scanf("%[^\n]%*c",input); 
		process_command(input);
	} while (input[0] != 'q' && game_not_ended());
	if (dead){
		printf("You Pressed On A Mine. GAME OVER\n\n");
		print_uncovered_field();
	} else if (all_mines_covered()){
		printf("All Mines Covered With Flags. You Won!\n");
	} else if (all_non_mines_shown()){
		printf("All non mines shown. You Won!\n");
	}
}





















