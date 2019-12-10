CC = gcc
OBJECTS = main.o GUI.o

all: $(OBJECTS)
	$(CC) main.o GUI.o -lSDL2 -o main

main.o: main.c macros.h 
	$(CC) main.c -c -o main.o

GUI.o: GUI.c GUI.h macros.h
	$(CC) GUI.c -c -o GUI.o


clean: 
	rm *.o $(objects)
