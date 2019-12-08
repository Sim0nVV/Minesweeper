#include "GUI.h"
#include "macros.h"

/*
 * Deze renderer wordt gebruikt om figuren in het venster te tekenen. De renderer
 * wordt geïnitialiseerd in de initialize_window-functie.
 */
static SDL_Renderer *renderer;

/* De afbeelding die een vakje met een "1" in voorstelt. */
static SDL_Texture *digit_1_texture;


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
int should_continue = 1;

/*
 * Dit is het venster dat getoond zal worden en waarin het speelveld weergegeven wordt.
 * Dit venster wordt aangemaakt bij het initialiseren van de GUI en wordt weer afgebroken
 * wanneer het spel ten einde komt.
 */
static SDL_Window *window;

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
		should_continue = 0;
		break;

	case SDL_MOUSEBUTTONDOWN:
		/*
		 * De speler heeft met de muis geklikt: met de onderstaande lijn worden de coördinaten in het
		 * het speelveld waar de speler geklikt heeft bewaard in de variabelen mouse_x en mouse_y.
		 */
		mouse_x = event.button.x;
		mouse_y = event.button.y;
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
	for(int w = 0; w < X_CELLS; w++){
	   for(int h = 0; h < Y_CELLS; h++){
	      SDL_Rect rectangle = {mouse_x +IMAGE_HEIGHT * h, mouse_y + IMAGE_WIDTH * w, IMAGE_WIDTH, IMAGE_HEIGHT };
	      SDL_RenderCopy(renderer, digit_1_texture, NULL, &rectangle);
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
	SDL_DestroyTexture(digit_1_texture);

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
	SDL_Surface* digit_1_texture_path = SDL_LoadBMP("Images/1.bmp");

	/*
	 * Zet deze afbeelding om naar een texture die getoond kan worden in het venster.
	 * Indien de texture niet kon omgezet worden, geeft de functie een NULL-pointer terug.
	 * */
	digit_1_texture = SDL_CreateTextureFromSurface(renderer, digit_1_texture_path);

	/* Dealloceer het SDL_Surface dat werd aangemaakt. */
	SDL_FreeSurface(digit_1_texture_path);
}

/*
 * Initialiseert onder het venster waarin het speelveld getoond zal worden, en de texture van de afbeelding die getoond zal worden.
 * Deze functie moet aangeroepen worden aan het begin van het spel, vooraleer je de spelwereld begint te tekenen.
 */
void initialize_gui() {
	initialize_window("Minesweeper");
	initialize_textures();
}

int main(int argc, char *argv[]) {
	initialize_gui();
	while (should_continue) {
		draw_window();
		read_input();
	}
	/* Dealloceer al het geheugen dat werd aangemaakt door SDL zelf. */
	free_gui();
	return 0;
}
