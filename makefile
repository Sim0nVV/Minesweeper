CC = gcc
OBJECTS = main.o GUI.o

all: $(OBJECTS)
	$(CC) main.o GUI.o -lSDL2 -o main

main.o: main.c GUI.h logic.h
	$(CC) main.c -c -o main.o

GUI.o: GUI.c GUI.h logic.h
	$(CC) GUI.c -c -o GUI.o


clean: 
	rm *.o $(objects)
