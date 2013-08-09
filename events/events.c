#include <stdio.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

typedef enum { false, true } bool;

// screen attributes
const int SCR_WIDTH = 640;
const int SCR_HEIGHT = 480;
const int SCR_BPP = 32;

bool init();
bool load_files();
void clean_up();
SDL_Surface *load_image(char *);
void apply_surface(int, int, SDL_Surface *, SDL_Surface *);

// the surfaces that will be used
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;

// the event structure that will be used
SDL_Event event;

int main(int argc, char *argv[])
{
    // make sure the program waits for a quit
    bool quit = false;

    // initialize
    if (!init()) {
	fprintf(stderr, "init() failed: %s\n", SDL_GetError());
	return 1;
    }
    // load images
    if (!load_files()) {
	fprintf(stderr, "load_files() failed: %s\n", SDL_GetError());
	return 1;
    }

    // blit image to screen
    apply_surface(0, 0, image, screen);
 
    // update screen. Quit if error in updating.
    if (SDL_Flip(screen) == -1) {
	fprintf(stderr, "SDL_Flip failed: %s\n", SDL_GetError());
	return 1;
    }

    while (!quit) {
	// while there's an event to handle
	while (SDL_PollEvent(&event)) {
	    // if user exited out the window
	    if (event.type == SDL_QUIT)
		quit = true;
	}
    }

    // free the image and quit SDL
    clean_up();
    return 0;
}

bool init() {
    // initialize all SDL subsystems. Return false if init fails.
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	return false;

    // set up screen
    screen = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BPP, SDL_SWSURFACE);
    // return false if error in setting up screen
    if (screen == NULL)
	return false;

    // set window caption
    SDL_WM_SetCaption("Event test", NULL);

    return true;
}

bool load_files() {
    // load the image
    image = load_image("x.png");
    // return false if there was an error in loading the image
    if (image == NULL)
	return false;

    return true;
}

void clean_up() {
    // free the image
    SDL_FreeSurface(image);

    // quit SDL
    SDL_Quit();
}

SDL_Surface *load_image(char *filename) {
    // temporary storage for the image that's loaded
    SDL_Surface *loaded_image = NULL;
    // the optimzed image that will be used
    SDL_Surface *optimized_image = NULL;

    // load the image. Bitmap will be 24-bit!
    loaded_image = IMG_Load(filename);

    // if nothing went wrong in loading the image
    if (loaded_image != NULL) {
	// create an optimzed image
	optimized_image = SDL_DisplayFormat(loaded_image);
	// free the old image from memory
	SDL_FreeSurface(loaded_image);
    }
    return optimized_image;
}

void apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dst) {
    // temporary rectangle to hold the offsets
    SDL_Rect offset;

    // give the offsets to the rectangle. Necessary as SDL_BlitSurface() only
    // accepts the offsets inside of an SDL_Rect.
    offset.x = x;
    offset.y = y;

    // blit the surface. Blitting = combining multiple bitmaps into one.
    SDL_BlitSurface(src, NULL, dst, &offset);
}
