#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

typedef enum {false, true} bool;

const int SCR_W = 640;
const int SCR_H = 320;
const int SCR_BPP = 32;

SDL_Surface *scr = NULL;
SDL_Surface *bg = NULL;
SDL_Surface *msg = NULL;
SDL_Surface *time_msg = NULL;

SDL_Event event;

SDL_Color color = {0x0, 0x0, 0x0};
TTF_Font *font = NULL;

SDL_Surface *load_img(char *);
void blit_surface(int, int, SDL_Surface *, SDL_Surface *);
bool init();
bool load_files();
void clean_up();

int main(int argc, char *argv[])
{
    bool quit = false;
    bool running = true;    // keeps track if timer is running or not
    Uint32 start = 0;	    // holds the timer starting time
    char time[20];	    // buffer that holds the time diff for the timer

    if (!init() || !load_files())
	return 1;
    msg = TTF_RenderText_Solid(font,"Press s to start or stop the timer",color);

    // start the timer. Notice how this comes after all the inits and loading.
    start = SDL_GetTicks();

    while (!quit) {
	while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT)
		quit = true;
	    if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_s) {
		    // if timer is running
		    if (running) {
			// stop the timer
			running = false;
			start = 0;
		    } else {
			// start the timer
			running = true;
			start = SDL_GetTicks();
		    }
		}
	    }
	}
	// blit the background to the screen
	blit_surface(0, 0, bg, scr);
	// blit the input messages to the screen
	blit_surface((SCR_W - msg->w)/2, (SCR_H - msg->h)/2, msg, scr);
	// if the timer is running
	if (running) {
	    sprintf(time, "time: %u s", (SDL_GetTicks()-start)/1000);
	    // blit time to screen
	    time_msg = TTF_RenderText_Solid(font,time,color);
	    blit_surface((SCR_W - time_msg->w)/2, 50, time_msg, scr);
	    SDL_FreeSurface(time_msg);
	}
	if (SDL_Flip(scr) == -1)
	    return 1;
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
    } if (TTF_Init() == -1) {
	fprintf(stderr, "TTF_Init() failed: %s\n", TTF_GetError());
	return false;
    } if ((scr=SDL_SetVideoMode(SCR_W, SCR_H, SCR_BPP, SDL_SWSURFACE))==NULL) {
	fprintf(stderr, "SDL_SetVideoMode() failed: %s\n", SDL_GetError());
	return false;
    }
    SDL_WM_SetCaption("Play some sounds!", NULL);
    return true;
}

bool load_files() {
    if ((bg=load_img("background.png")) == NULL) {
	fprintf(stderr, "load_img(\"background.png\") failed to load\n");
	return false;
    } if ((font=TTF_OpenFont("lazy.ttf", 30)) == NULL) {
	fprintf(stderr, "TTF_OpenFont() failed: %s\n", TTF_GetError());
	return false;
    }
    return true;
}

void clean_up() {
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_FreeSurface(bg);
    SDL_FreeSurface(msg);
    SDL_Quit();
}
