#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

typedef enum {false, true} bool;

const int SCR_WIDTH = 640;
const int SCR_HEIGHT = 480;
const int SCR_BPP = 32;

SDL_Surface *screen = NULL;
SDL_Surface *dots = NULL;
SDL_Event events;
SDL_Rect clip[4]; // holds the offsets and dimensions of the 4 dots sprites.

SDL_Surface *load_image(char *);
void key_image(SDL_Surface *);
void apply_clipped_surface(int, int, SDL_Surface *, SDL_Surface *, SDL_Rect *);

int main(int argc, char *argv[])
{
    bool quit = false;

    screen = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BPP, SDL_SWSURFACE);
    dots = load_image("dots.png");
    key_image(dots);

    // clip sizes for all dots. Same because dots are equal in size.
    int i;
    for (i = 0; i < 4; ++i) {
	clip[i].w = 100;
	clip[i].h = 100;
    }
    // clip range for the top left dot
    clip[0].x = 0;
    clip[0].y = 0;
    // clip range for the top right dot
    clip[1].x = 100;
    clip[1].y = 0;
    // clip range for the bottom left dot
    clip[2].x = 0;
    clip[2].y = 100;
    // clip range for the bottom right dot
    clip[3].x = 100;
    clip[3].y = 100;

    // fill the screen white
    Uint32 white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
    SDL_FillRect(screen, NULL, white);

    // blit images to screen
    apply_clipped_surface(0, 0, dots, screen, &clip[0]);
    apply_clipped_surface(540, 0, dots, screen, &clip[1]);
    apply_clipped_surface(0, 380, dots, screen, &clip[2]);
    apply_clipped_surface(540, 380, dots, screen, &clip[3]);

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

    SDL_FreeSurface(dots);
    SDL_Quit();

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

void key_image(SDL_Surface *img) {
    Uint32 color_key = SDL_MapRGB(img->format, 0, 0xFF, 0xFF);
    SDL_SetColorKey(img, SDL_SRCCOLORKEY, color_key);
}

void apply_clipped_surface(int x, int y, SDL_Surface *src, SDL_Surface *dst, SDL_Rect *clip) {
    // holds offsets
    SDL_Rect offsets;

    // get offsets
    offsets.x = x;
    offsets.y = y;

    // blit the piece of *src that we want onto *dst
    SDL_BlitSurface(src, clip, dst, &offsets);
}
