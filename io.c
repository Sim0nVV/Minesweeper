#include "cell.h"
#include "stdio.h"



void read_commandline_args(int argc, char *argv[]){
	int opt;
	char number;
	while(--argc > 0 && **++argv =='-')
		printf("first loop happens\n");
		while ((opt ==*++*argv) != '\0')
			printf("2nd loop\n");
			switch (opt){
				case 'w':
					number = **++argv;
					printf("%c\n", number);
					break;
				case 'h':
					number = **++argv;

					printf("%c\n", number);
					break;
				case 'm':
					number = **++argv;
					printf("%c\n", number);
					break;
				default:
					printf("illegal option, %c\n",opt);
						break;

			}


// branch naar init struct
  

// branc naar read txt file

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
void initialize_struct(int w, int h, int m){
	//st = (char *

}

