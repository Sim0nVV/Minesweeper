#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define  mines  		3
#define  x_value  		6
#define  y_value  		6

#define BOUNDARIES(i,j) 	if (i < 0 || i >= x_value || j < 0 || j >= y_value) { continue;}

struct Field{
   int mines_nearby;
   int mine;
   bool visible;
   bool flag;
};

struct Field grid[x_value][y_value] = {0, 0, false, false};

char return_char_on(int x, int y, bool visible){ // hier moeten type conversions gebeuren

   if(visible || grid[x][y].visible){
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
   for(int i = 0; i<x_value; i++){
      printf(" %i  ", i);
   }
   printf("\n");
}

void print_next_lines(bool visible){
   for(int y = 0; y<y_value; y++){
      printf("%i ", y);
      for(int x = 0; x<x_value; x++){
	 printf("| %c|", return_char_on(x,y,visible)); //Hangt ervanaf of het volledige veld zichtbaar is of niet
      }	
      printf("\n");
   }
}

void print_covered_field() {
   printf("Resterende vlaggen: %i\n", mines);
   print_first_line();
   print_next_lines(false); // boolean geeft aan of het hele veld getoond moet worden
}

void print_uncovered_field(){
   print_first_line();
   print_next_lines(true); // boolean geeft aan of hele veld geprint moet worden
}

void add_flag(int x,int y){
   grid[x][y].flag = true;
}

void reveal(int x, int y){
   grid[x][y].visible = true;
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

void initialize_grid(int n){

   int x = rand() % x_value; // code die random vakje zoekt, waar nog geen mijn is
   int y = rand() % y_value; 

   while(grid[x][y].mine){
      x = rand() % x_value;
      y = rand() % y_value;
   }
   printf("x = %i, y = %i\n", x , y);

   grid[x][y].mine = true;
   add_numbers_mine(x, y);

   if(n > 0){
      initialize_grid(n - 1);
   }
};

int main()
{
   srand(time(0));
   initialize_grid(mines);
   char input[6];
   while (input[0] != 'q' /*|| allvisible*/){
      scanf("%[^\n]%*c",input); //waarom geen pointer operator?
      process_command(input);
   }
   return 0;
}



















