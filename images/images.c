#include <stdio.h>

#include "SDL/SDL.h"

// screen attributes
const int SCR_WIDTH = 640;
const int SCR_HEIGHT = 480;
const int SCR_BPP = 32;

SDL_Surface *load_image(char[]);
void apply_surface(int, int, SDL_Surface *, SDL_Surface *);

int main(int argc, char *argv[])
{
    // the surfaces that will be used
    SDL_Surface *message = NULL;
    SDL_Surface *background = NULL;
    SDL_Surface *screen = NULL;

    // initialize all SDL subsystems. Abnormal return status if init fails.
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
	fprintf(stderr, "SDL_Init failed!\n");
	return 1;
    }

    // set up screen
    screen = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BPP, SDL_SWSURFACE);
    // abnormal return status if error in setting up screen
    if (screen == NULL) {
	fprintf(stderr, "SDL_SetVideoMode failed!\n");
	return 1;
    }

    // set window caption
    SDL_WM_SetCaption("Hello, world", NULL);

    // load images
    message = SDL_LoadBMP("hello.bmp");
    background = SDL_LoadBMP("background.bmp");

    // tile background to surface
    apply_surface(0, 0, background, screen);
    apply_surface(320, 0, background, screen);
    apply_surface(0, 240, background, screen);
    apply_surface(320, 240, background, screen);
    // also blit message to surface
    apply_surface(180, 140, message, screen);

    // update screen. Quit if error in updating.
    if (SDL_Flip(screen) == -1) {
	fprintf(stderr, "SDL_Flip failed!\n");
	return 1;
    }

    // pause so window doesn't immediately disappear. Arg is in millis.
    SDL_Delay(10000);

    // free the loaded images
    SDL_FreeSurface(message);
    SDL_FreeSurface(background);

    // quit SDL
    SDL_Quit();

    return 0;
}

SDL_Surface *load_image(char filename[]) {
    // temporary storage for the image that's loaded
    SDL_Surface *loaded_image = NULL;
    // the optimzed image that will be used
    SDL_Surface *optimized_image = NULL;

    // load the image. Bitmap will be 24-bit!
    loaded_image = SDL_LoadBMP(filename);

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
