#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define  mines  3
#define  x_value  5
#define  y_value  5

#define flag_value 2000
#define visible_value 1000
#define mine_value -1

// -1 = Mine,   +1000 = visible, +2000 = flag (Can't happen at same time)

int matrix[x_value][y_value] = {0};


char return_char_on(int x, int y){ // hier moeten type conversions gebeuren
//   printf("%i", matrix[x][y]);
   
   if (matrix[x][y] / flag_value == 2 || matrix[x][y] == 1999){
      return 'F';
   } else if (matrix[x][y] == mine_value){
      return 'M';
   } else if ((matrix[x][y] - 1) <= (9 - 1)){
      return matrix[x][y] +48;
   }
    return '0';
}

void print_first_line(){
   printf("   ");
   for(int i = 0; i<x_value; i++){
      printf(" %i  ", i);
   }
   printf("\n");
}

void print_next_lines(int visible){
   for(int y = 0; y<y_value; y++){
      printf("%i ", y);
      for(int x = 0; x<x_value; x++){
	 if (visible){
	    printf("| %c|", return_char_on(x,y)); //Hangt ervanaf of het volledige veld zichtbaar is of niet
	 } else {
	    printf("| %c|", return_char_on(x,y));	
	 }
      }	
      printf("\n");
   }
}

void print_covered_field() {
   printf("Resterende vlaggen: %i\n", mines);
   print_first_line();
   print_next_lines(0);
}

void print_uncovered_field(){
   print_covered_field(); //voorlopige code
}

void add_flag(int x,int y){
   if (matrix[x][y] % 1000 != 999 ||  ((matrix[x][y] - 1) <= (9 - 1))){
      matrix[x][y] = matrix[x][y] + flag_value;
   }
}


void process_command(char *input){
   char command = input[0]; // probleem met uitlezen string
   switch (command){
      case 'P':
	 print_covered_field();
	 break;
      case 'F':
	 add_flag(input[2] - 48,input[4] - 48); //We geven geen getallen in groter dan 9
	 print_covered_field();
	 break;
      case 'R':
	 //	 reveal(input[2],input[4]);
	 print_covered_field();
	 break;
      default:
	 printf("Not a known commando\n");
   }
}

void add_numbers_mine(int x, int y){ //patroon komt ook voor bij onthullen
   for(int i = x - 1; i < x + 2; i++){ // hij gaat erover. Mag niet over grenzen gaan
      for(int j = y - 1; j< y + 2; j++){
	 if(matrix[i][j] == mine_value){
	    continue;
	 } else {
	    matrix[i][j] += 1;
	 }
      }
   }

}

void initialize_matrix(int n){
   
   int x = rand() % x_value;
   int y = rand() % y_value;

   while(matrix[x][y] == mine_value){
      x = rand() % x_value;
      y = rand() % y_value;
   }
   printf("x = %i, y = %i\n", x , y);

   matrix[x][y] = mine_value;
   add_numbers_mine(x, y);

   if(n > 0){

      initialize_matrix(n - 1);
   }
};

int main()
{
   srand(time(0));
   initialize_matrix(mines);
   char input[6];
   while (input[0] != 'q'){
      scanf("%[^\n]%*c",input); //waarom geen pointer operator?
      process_command(input);
   }
   return 0;
}



















