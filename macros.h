#ifndef MACROS_H
#define MACROS_H

#define  MINES  		5
#define  X_VALUE  		6
#define  Y_VALUE 		6

#define BOUNDARIES(i,j) 	if (i < 0 || i >= X_VALUE || j < 0 || j >= Y_VALUE) { continue;}
#define INPUT_TO_INT(i)		input[i] - 48
#define SINGLE_DIGIT(i) 	(0 <= i && i <= X_VALUE)

#endif
