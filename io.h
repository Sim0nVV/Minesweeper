#ifndef IO_H_
#define IO_H_

#include "cell.h"
#include "logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_commandline_args(int argc, char *argv[]);
void free_struct();
void save_to_file(char *path);

#endif /* IO_H_ */
