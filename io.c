#include "cell.h"
#include "stdio.h"

void initialize_struct(int w, int h){
	//malloc(sizeof(int) +
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
		printf("The ARGC is %i\n", argc);
		**++argv;
		--argc;

	}

	// branch naar init struct
	initialize_struct(width,height);


	// branc naar read txt file
	return mines;

}

void malloc_struct(){

}

/*
   void output_curr_iter(){

   }

   void read_txt_file(){

   }
   void free_struct(){

   }


*/

