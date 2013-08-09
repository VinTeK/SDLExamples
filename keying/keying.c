#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

typedef enum {false, true} bool;

const int SCR_WIDTH = 640;
const int SCR_HEIGHT = 480;
const int SCR_BPP = 32;

SDL_Surface *load_image(char *);
void key_image(SDL_Surface *);
void apply_surface(int, int, SDL_Surface *, SDL_Surface *);

SDL_Event events;

int main(int argc, char *argv[]) 
{
    bool quit = false;
    SDL_Surface *screen = NULL;
    SDL_Surface *background = NULL;
    SDL_Surface *foo = NULL;

    screen = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BPP, SDL_SWSURFACE);
    background = load_image("background.png");
    foo = load_image("foo.png");
    key_image(foo);

    SDL_WM_SetCaption("Keying images!", NULL);

    apply_surface(0, 0, background, screen);
    apply_surface(240, 190, foo, screen);

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

    SDL_FreeSurface(background);
    SDL_FreeSurface(foo);

    SDL_Quit();

    return 0;
}

SDL_Surface *load_image(char *filename) {
    SDL_Surface *loaded_image = NULL;
    SDL_Surface *optimized_image = NULL;

    loaded_image = IMG_Load(filename);

    // if image loaded successfully
    if (loaded_image != NULL) {
	optimized_image = SDL_DisplayFormat(loaded_image);
	SDL_FreeSurface(loaded_image);
    }
    return optimized_image;
}

void key_image(SDL_Surface *img) {
    // map the color key
    Uint32 color_key = SDL_MapRGB(img->format, 0, 0xFF, 0xFF);
    // set the pixels of color R:0, G:0xFF, B:0xFF to be transparent
    SDL_SetColorKey(img, SDL_SRCCOLORKEY, color_key);
}

void apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dst) {
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(src, NULL, dst, &offset);
}


