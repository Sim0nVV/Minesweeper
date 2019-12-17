#ifndef GUI_H_
#define GUI_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*
 * Importeer de benodigde functies uit SDL2.
 */
#include <SDL2/SDL.h>

/*
 * De hoogte en breedte (in pixels) van de afbeeldingen voor de vakjes in het speelveld die getoond worden.
 * Als je andere afbeelding wil gebruiken in je GUI, zorg er dan voor dat deze
 * dimensies ook aangepast worden.
 */
#define IMAGE_HEIGHT 50
#define IMAGE_WIDTH 50

extern void initialize_gui();
extern void draw_window();
extern void free_gui();

extern void read_input();
extern void read_init_input();

bool initialized_grid;


bool pressed_quit;




#endif /* GUI_H_ */
