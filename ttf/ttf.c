#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

typedef enum {false, true} bool;

const int SCR_WIDTH = 640;
const int SCR_HEIGHT = 480;
const int SCR_BPP = 32;

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *message = NULL;

SDL_Event events;

// the font that's going to be used
TTF_Font *font = NULL;

// the color of the text
SDL_Color text_color = {0xFF, 0xFF, 0xFF};

SDL_Surface *load_image(char *);
void apply_surface(int, int, SDL_Surface *, SDL_Surface *);
bool init();
bool load_files();
void clean_up();

int main(int argc, char *argv[])
{
    bool quit = false;

    // initialize and load files
    if (!init() || !load_files())
	return 1;

    // render the text
    message = TTF_RenderText_Solid(font, "Making bacon pancakes", text_color);
    if (message == NULL) {
	fprintf(stderr, "TTF_RenderText_Solid failed!: %s\n", SDL_GetError());
	return 1;
    }

    // blit surfaces to screen
    apply_surface(0, 0, background, screen);
    apply_surface(200, 200, message, screen);

    // update the screen
    if (SDL_Flip(screen) == -1) {
	fprintf(stderr, "SDL_Flip failed!: %s\n", SDL_GetError());
	return 1;
    }

    while (!quit) {
	while (SDL_PollEvent(&events)) {
	    if (events.type == SDL_QUIT)
		quit = true;
	}
    }

    clean_up();
    return 0;
}

SDL_Surface *load_image(char *filename) {
    SDL_Surface *loaded_image = IMG_Load(filename);
    SDL_Surface *optimized_image = NULL;

    if (loaded_image != NULL) {
	optimized_image = SDL_DisplayFormat(loaded_image);
	SDL_FreeSurface(loaded_image);
    }
    return optimized_image;
}

void apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dst) {
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(src, NULL, dst, &offset);
}

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	return false;

    screen = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BPP, SDL_SWSURFACE);
    if (screen == NULL)
	return false;

    // initialize SDL_ttf
    if (TTF_Init() == -1)
	return false;

    SDL_WM_SetCaption("TTF Test", NULL);
    return true;
}

bool load_files() {
    background = load_image("background.png");
    if (background == NULL)
	return false;

    // open the font "lazy.ttf" at size 28
    font = TTF_OpenFont("lazy.ttf", 28);
    if (font == NULL)
	return false;

    return true;
}

void clean_up() {
    SDL_FreeSurface(background);
    SDL_FreeSurface(message);
    // close the font that was used
    TTF_CloseFont(font);
    // quit SDL_ttf
    TTF_Quit();
    // quit SDL
    SDL_Quit();
}
