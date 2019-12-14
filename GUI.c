#include "GUI.h"
#include "logic.h"
#include "cell.h"

/*
 * Deze renderer wordt gebruikt om figuren in het venster te tekenen. De renderer
 * wordt geïnitialiseerd in de initialize_window-functie.
 */
SDL_Renderer *renderer;

/* De afbeelding die een vakje met een "1" in voorstelt. */
SDL_Texture *digit_0_texture;
SDL_Texture *digit_1_texture;
SDL_Texture *digit_2_texture;
SDL_Texture *digit_3_texture;
SDL_Texture *digit_4_texture;
SDL_Texture *digit_5_texture;
SDL_Texture *digit_6_texture;
SDL_Texture *digit_7_texture;
SDL_Texture *digit_8_texture;
SDL_Texture *covered_texture;
SDL_Texture *flagged_texture;


/*
 * Onderstaande twee lijnen maken deel uit van de minimalistische voorbeeldapplicatie:
 * ze houden de laatste positie bij waar de gebruiker geklikt heeft.
 */
int mouse_x = 0;
int mouse_y = 0;

/*
 * Geeft aan of de applicatie moet verdergaan.
 * Dit is waar zolang de gebruiker de applicatie niet wilt afsluiten door op het kruisje te klikken.
 */
bool pressed_quit = false;
bool initialized_grid = false;

/*
 * Dit is het venster dat getoond zal worden en waarin het speelveld weergegeven wordt.
 * Dit venster wordt aangemaakt bij het initialiseren van de GUI en wordt weer afgebroken
 * wanneer het spel ten einde komt.
 */
SDL_Window *window;

int is_relevant_event(SDL_Event *event) {
	if (event == NULL) {
		return 0;
	}
	return (event->type == SDL_MOUSEBUTTONDOWN) || (event->type == SDL_QUIT);
}

/*
 * Vangt de input uit de GUI op. Deze functie is al deels geïmplementeerd, maar je moet die zelf
 * nog afwerken. Je mag natuurlijk alles aanpassen aan deze functie, inclusies return-type en argumenten.
 */
void read_init_input(int mines) {
	SDL_Event event;

	while (! (SDL_PollEvent(&event) && is_relevant_event(&event))) {}

	switch (event.type) {
		case SDL_QUIT:
			pressed_quit = true;
			break;

		case SDL_MOUSEBUTTONDOWN:
			/*
			 * De speler heeft met de muis geklikt: met de onderstaande lijn worden de coördinaten in het
			 * het speelveld waar de speler geklikt heeft bewaard in de variabelen mouse_x en mouse_y.
			 */
			mouse_x = event.button.x;
			mouse_y = event.button.y;
			int x = mouse_x / IMAGE_WIDTH;
			int y = mouse_y / IMAGE_HEIGHT;
			printf("entered read init input: Gui.c 59\n");
			printf("Clicked\n");

			initialize_grid(mines, x, y);
			printf("After initialize grid\n");
			initialized_grid = true;


			break;
	}
}
void read_input() {
	SDL_Event event;

	/*
	 * Handelt alle input uit de GUI af.
	 * Telkens de speler een input in de GUI geeft (bv. een muisklik, muis bewegen, toets indrukken enz.)
	 * wordt er een 'event' (van het type SDL_Event) gegenereerd dat hier wordt afgehandeld.
	 *
	 * Niet al deze events zijn relevant voor jou: als de muis bv. gewoon wordt bewogen, hoef
	 * je niet te reageren op dit event.
	 * We gebruiken daarom de is_relevant_event-functie die niet-gebruikte events wegfiltert.
	 *
	 * Zie ook https://wiki.libsdl.org/SDL_PollEvent en http://www.parallelrealities.co.uk/2011_09_01_archive.html
	 */
	while (! (SDL_PollEvent(&event) && is_relevant_event(&event))) {}

	switch (event.type) {
		case SDL_QUIT:
			/* De gebruiker heeft op het kruisje van het venster geklikt om de applicatie te stoppen. */
			pressed_quit = true;
			break;

		case SDL_MOUSEBUTTONDOWN:
			/*
			 * De speler heeft met de muis geklikt: met de onderstaande lijn worden de coördinaten in het
			 * het speelveld waar de speler geklikt heeft bewaard in de variabelen mouse_x en mouse_y.
			 */
			mouse_x = event.button.x;
			mouse_y = event.button.y;
			int x = mouse_x / IMAGE_WIDTH;
			int y = mouse_y / IMAGE_HEIGHT;

			if(event.button.button == SDL_BUTTON_RIGHT){
				toggle_flag(x,y);
			} else {
				reveal(x,y);

			}
			break;
	}
}

void draw_window() {
	/*
	 * Maakt het venster blanco.
	 */
	SDL_RenderClear(renderer);

	/*
	 * Bereken de plaats (t.t.z., de rechthoek) waar een afbeelding moet getekend worden.
	 * Dit is op de plaats waar de gebruiker het laatst geklikt heeft.
	 */
	/* Tekent de afbeelding op die plaats. */
	SDL_Rect rectangle;
	SDL_Texture * texture;
	for(int x = 0; x < game->width; x++){
		for(int y = 0; y < game->height; y++){
			printf("loop happens \n");
			SDL_Rect rectangle = {IMAGE_WIDTH * x, IMAGE_HEIGHT * y, IMAGE_WIDTH, IMAGE_HEIGHT };
			if(initialized_grid){
				char c = return_char_on(x,y,false);
				printf("after ret char\n");
				char c_int = c - '0';
				if(c == 'F'){
					texture = flagged_texture;
				}else if (c == ' '){
					texture = covered_texture;
				}else if (c_int >= 0 && c_int < 9){
					switch(c_int){
						case 0: texture = digit_0_texture; break;
						case 1: texture = digit_1_texture; break;
						case 2: texture = digit_2_texture; break;
						case 3: texture = digit_3_texture; break;
						case 4: texture = digit_4_texture; break;
						case 5: texture = digit_5_texture; break;
						case 6: texture = digit_6_texture; break;
						case 7: texture = digit_7_texture; break;
						case 8: texture = digit_8_texture; break;

					}
				}
			} else {
				texture = covered_texture;
			}
		SDL_RenderCopy(renderer, texture, NULL, &rectangle);
		}
	}


	/*
	 * Onderstaande code moet zeker worden uitgevoerd op het einde van deze functie.
	 * Wanneer je iets tekent in het venster wordt dit venster nl. niet meteen aangepast.
	 * Via de SDL_RenderPresent functie wordt het venster correct geüpdatet.
	 */
	SDL_RenderPresent(renderer);
}

/*
 * Initialiseert het venster en alle extra structuren die nodig zijn om het venster te manipuleren.
 */
void initialize_window(const char *title) {
	/*
	 * Code o.a. gebaseerd op:
	 * http://lazyfoo.net/tutorials/SDL/02_getting_an_image_on_the_screen/index.php
	 */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	/* Maak het venster aan met de gegeven dimensies en de gegeven titel. */
	window = SDL_CreateWindow(title, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		/* Er ging iets verkeerd bij het initialiseren. */
		printf("Couldn't set screen mode to required dimensions: %s\n", SDL_GetError());
		exit(1);
	}

	/* Initialiseert de renderer. */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	/* Laat de default-kleur die de renderer in het venster tekent wit zijn. */
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

/*
 * Dealloceert alle SDL structuren die geïnitialiseerd werden.
 */
void free_gui() {
	/* Dealloceert de SDL_Textures die werden aangemaakt. */
	SDL_DestroyTexture(digit_0_texture);
	SDL_DestroyTexture(digit_1_texture);
	SDL_DestroyTexture(digit_2_texture);
	SDL_DestroyTexture(digit_3_texture);
	SDL_DestroyTexture(digit_4_texture);
	SDL_DestroyTexture(digit_5_texture);
	SDL_DestroyTexture(digit_6_texture);
	SDL_DestroyTexture(digit_7_texture);
	SDL_DestroyTexture(digit_8_texture);
	SDL_DestroyTexture(covered_texture);
	SDL_DestroyTexture(flagged_texture);

	/* Dealloceert het venster. */
	SDL_DestroyWindow(window);
	/* Dealloceert de renderer. */
	SDL_DestroyRenderer(renderer);

	/* Sluit SDL af. */
	SDL_Quit();
}

/*
 * Laadt alle afbeeldingen die getoond moeten worden in.
 */
void initialize_textures() {

	/*
	 * Laadt de afbeeldingen in. In deze minimalistische applicatie laden we slechts 1 afbeelding in.
	 * Indien de afbeelding niet kon geladen worden (bv. omdat het pad naar de afbeelding verkeerd is),
	 * geeft SDL_LoadBMP een NULL-pointer terug.
	 */
	SDL_Surface* digit_0_texture_path = SDL_LoadBMP("Images/0.bmp");
	SDL_Surface* digit_1_texture_path = SDL_LoadBMP("Images/1.bmp");
	SDL_Surface* digit_2_texture_path = SDL_LoadBMP("Images/2.bmp");
	SDL_Surface* digit_3_texture_path = SDL_LoadBMP("Images/3.bmp");
	SDL_Surface* digit_4_texture_path = SDL_LoadBMP("Images/4.bmp");
	SDL_Surface* digit_5_texture_path = SDL_LoadBMP("Images/5.bmp");
	SDL_Surface* digit_6_texture_path = SDL_LoadBMP("Images/6.bmp");
	SDL_Surface* digit_7_texture_path = SDL_LoadBMP("Images/7.bmp");
	SDL_Surface* digit_8_texture_path = SDL_LoadBMP("Images/8.bmp");
	SDL_Surface* covered_texture_path = SDL_LoadBMP("Images/covered.bmp");
	SDL_Surface* flagged_texture_path = SDL_LoadBMP("Images/flagged.bmp");


	/*
	 * Zet deze afbeelding om naar een texture die getoond kan worden in het venster.
	 * Indien de texture niet kon omgezet worden, geeft de functie een NULL-pointer terug.
	 * */
	digit_0_texture = SDL_CreateTextureFromSurface(renderer, digit_0_texture_path);
	digit_1_texture = SDL_CreateTextureFromSurface(renderer, digit_1_texture_path);
	digit_2_texture = SDL_CreateTextureFromSurface(renderer, digit_2_texture_path);
	digit_3_texture = SDL_CreateTextureFromSurface(renderer, digit_3_texture_path);
	digit_4_texture = SDL_CreateTextureFromSurface(renderer, digit_4_texture_path);
	digit_5_texture = SDL_CreateTextureFromSurface(renderer, digit_5_texture_path);
	digit_6_texture = SDL_CreateTextureFromSurface(renderer, digit_6_texture_path);
	digit_7_texture = SDL_CreateTextureFromSurface(renderer, digit_7_texture_path);
	digit_8_texture = SDL_CreateTextureFromSurface(renderer, digit_8_texture_path);
	flagged_texture = SDL_CreateTextureFromSurface(renderer, flagged_texture_path);
	covered_texture = SDL_CreateTextureFromSurface(renderer, covered_texture_path);

	/* Dealloceer het SDL_Surface dat werd aangemaakt. */
	SDL_FreeSurface(digit_0_texture_path);
	SDL_FreeSurface(digit_1_texture_path);
	SDL_FreeSurface(digit_2_texture_path);
	SDL_FreeSurface(digit_3_texture_path);
	SDL_FreeSurface(digit_4_texture_path);
	SDL_FreeSurface(digit_5_texture_path);
	SDL_FreeSurface(digit_6_texture_path);
	SDL_FreeSurface(digit_7_texture_path);
	SDL_FreeSurface(digit_8_texture_path);
	SDL_FreeSurface(covered_texture_path);
	SDL_FreeSurface(flagged_texture_path);
}

/*
 * Initialiseert onder het venster waarin het speelveld getoond zal worden, en de texture van de afbeelding die getoond zal worden.
 * Deze functie moet aangeroepen worden aan het begin van het spel, vooraleer je de spelwereld begint te tekenen.
 */
void initialize_gui() {
	initialize_window("Minesweeper");
	initialize_textures();
}

