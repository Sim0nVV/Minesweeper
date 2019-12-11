CC = gcc
OBJECTS = main.o GUI.o logic.o

all: $(OBJECTS)
	$(CC) $(OBJECTS) -lSDL2 -o main

main.o: main.c GUI.h logic.h 
	$(CC) main.c -c -o main.o

GUI.o: GUI.c GUI.h logic.h cell.h
	$(CC) GUI.c -c -o GUI.o

logic.o: logic.c logic.h cell.h
	$(CC) logic.c -c -o logic.o


clean: 
	rm *.o $(objects)
