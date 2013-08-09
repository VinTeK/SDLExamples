#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include "timer.h"

const int SCR_W = 640;
const int SCR_H = 320;
const int SCR_BPP = 32;
const int FRAMES_PER_SEC = 20;	// global constant for frames per second

SDL_Surface *scr = NULL;
SDL_Surface *bg = NULL;
SDL_Surface *msg = NULL;

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
    bool cap = true;		// whether or not to cap the framerate
    int frame = 0;		// keep track of current frame
    Timer fps = new_timer();	// our timer

    if (!init() || !load_files())
	return 1;
    msg = TTF_RenderText_Solid(font, "20 fps", color);

    while (!quit) {
	// start the frame timer
	start_timer(&fps);

	while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_f)
		    // switch frame capping on/off
		    cap = !cap;
		else if (event.key.keysym.sym == SDLK_q)
		    quit = true;
	    } else if (event.type == SDL_QUIT)
		quit = true;
	}
	blit_surface(0, 0, bg, scr);

	// blit message based on fps
	blit_surface((SCR_W - msg->w)/2, ((SCR_H+msg->h*2)/FRAMES_PER_SEC) * (frame%FRAMES_PER_SEC) - msg->h, msg, scr);
	
    	if (SDL_Flip(scr) == -1) {
	    fprintf(stderr, "SDL_Flip() failed: %s\n", SDL_GetError());
	    return 1;
	}
	// don't forget to increment the frame counter!
	++frame;

	// if we want to cap the framerate
	if (cap && get_ticks(&fps) < 1000/FRAMES_PER_SEC) {
	    // sleep the remaining frame time
	    SDL_Delay(1000/FRAMES_PER_SEC - get_ticks(&fps));
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
    SDL_FreeSurface(msg);
    SDL_Quit();
}
