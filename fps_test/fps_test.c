#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "timer.h"

const int SCR_W = 640;
const int SCR_H = 480;
const int SCR_BPP = 32;
const int FRAMES_PER_SEC = 20;	// global constant for frames per second

SDL_Surface *scr = NULL;
SDL_Surface *bg = NULL;

SDL_Event event;

SDL_Surface *load_img(char *);
void blit_surface(int, int, SDL_Surface *, SDL_Surface *);
bool init();
bool load_files();
void clean_up();

int main(int argc, char *argv[])
{
    bool quit = false;
    int frame = 0;		// keep track of current frame
    Timer fps = new_timer();	// timer used to calculate the fps
    Timer update = new_timer();	// timer used to update the caption

    if (!init() || !load_files())
	return 1;

    start_timer(&update);
    start_timer(&fps);

    while (!quit) {
	while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT)
		quit = true;
	}
	blit_surface(0, 0, bg, scr);

    	if (SDL_Flip(scr) == -1) {
	    fprintf(stderr, "SDL_Flip() failed: %s\n", SDL_GetError());
	    return 1;
	}
	// increment the frame counter
	++frame;

	// if a second has passed since the caption was last updated
	if (get_ticks(&update) > 1000) {
	    // the framerate as a string
	    char str[20];
	    // calculate the fps to display as string
	    snprintf(str,20,"Average fps: %f",frame / (get_ticks(&fps)/1000.f));
	    // reset the caption
	    SDL_WM_SetCaption(str, NULL);
	    // restart the update timer
	    start_timer(&update);
	}
    }

    clean_up();
    return 0;
}

SDL_Surface *load_img(char *filename) {
    SDL_Surface *img = NULL;
    if ((img=IMG_Load(filename)) != NULL)
	img = SDL_DisplayFormat(img);
    return img;
}

void blit_surface(int x, int y, SDL_Surface *src, SDL_Surface *dst) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(src, NULL, dst, &offset);
}

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
	fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
	return false;
    }
    if ((scr=SDL_SetVideoMode(SCR_W, SCR_H, SCR_BPP, SDL_SWSURFACE))==NULL) {
	fprintf(stderr, "SDL_SetVideoMode() failed: %s\n", SDL_GetError());
	return false;
    }
    SDL_WM_SetCaption("advanced timer", NULL);
    return true;
}

bool load_files() {
    if ((bg=load_img("testing.png")) == NULL) {
	fprintf(stderr, "load_img(\"testing.png\") failed to load\n");
	return false;
    }
    return true;
}

void clean_up() {
    SDL_FreeSurface(bg);
    SDL_Quit();
}
