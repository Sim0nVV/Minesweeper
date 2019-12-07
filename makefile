all: main.o 
	gcc main.o -o main

main.o: main.c macros.h
	gcc main.c -c -o main.o
