#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

typedef enum {false, true} bool;

typedef struct {
    int start_ticks;    // clock time when the timer started
    int paused_ticks;   // ticks stored when timer was paused
    bool started;   // timer started flag
    bool paused;    // timer paused flag
} Timer;

const int SCR_W = 640;
const int SCR_H = 320;
const int SCR_BPP = 32;

SDL_Surface *scr = NULL;
SDL_Surface *bg = NULL;
SDL_Surface *msg1 = NULL;
SDL_Surface *msg2 = NULL;
SDL_Surface *time_msg = NULL;

SDL_Event event;

SDL_Color color = {0x0, 0x0, 0x0};
TTF_Font *font = NULL;

// timer.c functions
Timer new_timer();
void start_timer(Timer *);
void stop_timer(Timer *);
void pause_timer(Timer *);
void unpause_timer(Timer *);
int get_ticks(Timer *);

SDL_Surface *load_img(char *);
void blit_surface(int, int, SDL_Surface *, SDL_Surface *);
bool init();
bool load_files();
void clean_up();

int main(int argc, char *argv[])
{
    bool quit = false;
    char time[20];  // buffer for storing digits of ticks
    Timer timer = new_timer();    // our timer

    if (!init() || !load_files())
	return 1;
    msg1 = TTF_RenderText_Solid(font, "Press s to start/stop", color);
    msg2 = TTF_RenderText_Solid(font, "Press p to pause/unpaused", color);
    // start the timer
    start_timer(&timer);

    while (!quit) {
	while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_s) {
		    // if timer is running
		    if (timer.started)
			// stop the timer
			stop_timer(&timer);
		    else
			// start the timer
			start_timer(&timer);
		} else if (event.key.keysym.sym == SDLK_p) {
		    if (timer.paused)
			unpause_timer(&timer);
		    else
			pause_timer(&timer);
		}
	    } else if (event.type == SDL_QUIT)
		quit = true;
	}
	// blit the background to the screen
	blit_surface(0, 0, bg, scr);
	// blit the input messages to the screen
	blit_surface((SCR_W - msg1->w)/2, (SCR_H - msg1->h)/2-25, msg1, scr);
	blit_surface((SCR_W - msg2->w)/2, (SCR_H - msg2->h)/2+25, msg2, scr);
	// if the timer is running
	if (timer.started) {
	    sprintf(time, "time: %d", get_ticks(&timer));
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
    SDL_WM_SetCaption("advanced timer", NULL);
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
    SDL_FreeSurface(msg1);
    SDL_FreeSurface(msg2);
    SDL_Quit();
}
