#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define  MINES  		5
#define  X_VALUE  		6
#define  Y_VALUE 		6

#define BOUNDARIES(i,j) 	if (i < 0 || i >= X_VALUE || j < 0 || j >= Y_VALUE) { continue;}
int flags_planted = MINES;
bool dead = false;

struct Field{
   int mines_nearby;
   int mine;
   bool visible;
   bool flag;
};

struct Field grid[X_VALUE][Y_VALUE] = {0, 0, false, false};

char return_char_on(int x, int y, bool visible){ // hier moeten type conversions gebeuren
   if (grid[x][y].flag){
      return 'F';
   } else if(visible || grid[x][y].visible){
      if (grid[x][y].flag){
	 return 'F';
      } else if (grid[x][y].mine){
	 return 'M';
      } else {
	 return grid[x][y].mines_nearby +48;
      }}else { 
	 return ' ';
      }
}

void print_first_line(){
   printf("   ");
   for(int i = 0; i<X_VALUE; i++){
      printf(" %i  ", i);
   }
   printf("\n");
}

void print_next_lines(bool visible){
   for(int y = 0; y<Y_VALUE; y++){
      printf("%i ", y);
      for(int x = 0; x<X_VALUE; x++){
	 printf("| %c|", return_char_on(x,y,visible)); //Hangt ervanaf of het volledige veld zichtbaar is of niet
      }	
      printf("\n");
   }
}

void print_covered_field() {
   printf("\nRemaining vlaggen: %i\n\n", flags_planted);
   print_first_line();
   print_next_lines(false); // boolean geeft aan of het hele veld getoond moet worden
}

void print_uncovered_field(){
   print_first_line();
   print_next_lines(true); // boolean geeft aan of hele veld geprint moet worden
}

void add_flag(int x,int y){
   if(flags_planted == 0){
      printf("Maximum amount of planted flags reached.\n");
   } else if(grid[x][y].flag) {
      grid[x][y].flag = !grid[x][y].flag;
      flags_planted += 1;
   } else {
      grid[x][y].flag = !grid[x][y].flag;
      flags_planted -= 1;
   }
}

void reveal(int x, int y){
   if(!grid[x][y].flag){
      if(grid[x][y].mines_nearby > 0 && !grid[x][y].mine){
	 grid[x][y].visible = true;
      } else if (grid[x][y].mine){
	 dead = true;
      } else {
	 for(int i = x - 1; i < x + 2; i++){ // hij gaat erover. Mag niet over grenzen gaan
	    for(int j = y - 1; j< y + 2; j++){
	       BOUNDARIES(i,j)
		  if(grid[i][j].visible || grid[i][j].mine){
		     continue;
		  } else if (grid[i][j].mines_nearby > 0) {
		     grid[i][j].visible = true;
		  } else {
		     grid[i][j].visible = true;
		     reveal(i,j);
		  }
	    }
	 }

      }
   }
}


void process_command(char *input){
   char command = input[0]; // probleem met uitlezen string
   switch (command){
      case 'P':
	 print_uncovered_field();
	 break;
      case 'F':
	 add_flag(input[2] - 48,input[4] - 48); //We geven geen getallen in groter dan 9
	 print_covered_field();
	 break;
      case 'R':
	 reveal(input[2] - 48,input[4] - 48);
	 print_covered_field();
	 break;
      default:
	 printf("Not a known commando\n");
   }
}

void add_numbers_mine(int x, int y){ //patroon komt ook voor bij onthullen
   for(int i = x - 1; i < x + 2; i++){ // hij gaat erover. Mag niet over grenzen gaan
      for(int j = y - 1; j< y + 2; j++){
	 BOUNDARIES(i,j)
	    if(grid[i][j].mine){
	       continue;
	    } else {
	       grid[i][j].mines_nearby += 1;
	    }
      }
   }
}
bool all_mines_covered(){
   bool check = true;
   for(int x = 0; x< X_VALUE; x++){
      for(int y = 0; y<Y_VALUE; y++){
	 if (grid[x][y].mine){
	    check = check && grid[x][y].flag;
	 }
      }
   }
   return check;
}

void initialize_grid(int n){

   int x = rand() % X_VALUE; // code die random vakje zoekt, waar nog geen mijn is
   int y = rand() % Y_VALUE; 

   while(grid[x][y].mine){
      x = rand() % X_VALUE;
      y = rand() % Y_VALUE;
   }

   grid[x][y].mine = true;
   add_numbers_mine(x, y);

   if(n > 1){
      initialize_grid(n - 1);
   }
}

int main()
{
   srand(time(0));
   initialize_grid(MINES);
   char input[6];
   while (input[0] != 'q' && !all_mines_covered() && !dead){
      scanf("%[^\n]%*c",input); //waarom geen pointer operator?
      process_command(input);
   } 
   if (dead){
      printf("You Pressed On A Mine. GAME OVER\n");
   } else if (all_mines_covered()){
      printf("All Mines Covered With Flags. You Won!\n");
   }
   return 0;
}



















