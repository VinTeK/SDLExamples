#include "motion.h"
#include "dot.h"

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

    if (!init() || !load_files())
	return 1;

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
