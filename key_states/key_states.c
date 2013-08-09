#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

typedef enum {false, true} bool;

const int SCR_W = 640;
const int SCR_H = 480;
const int SCR_BPP = 32;

SDL_Surface *scr = NULL;
SDL_Surface *bg = NULL;

// messages to display upon keystrokes
SDL_Surface *up = NULL;
SDL_Surface *down = NULL;
SDL_Surface *left = NULL;
SDL_Surface *right = NULL;

SDL_Event event;
// holds the pointer to an array reflecting the current keyboard state
Uint8 *keystate = NULL;

SDL_Color txt_color = {0x0, 0x0, 0x0};
TTF_Font *font = NULL;

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

    // render the text
    up = TTF_RenderText_Solid(font, "up", txt_color);
    down = TTF_RenderText_Solid(font, "down", txt_color);
    left = TTF_RenderText_Solid(font, "left", txt_color);
    right = TTF_RenderText_Solid(font, "right", txt_color);

    while (!quit) {
	while (SDL_PollEvent(&event))
	    if (event.type == SDL_QUIT)
		quit = true;

	// draw background
	blit_surface(0, 0, bg, scr);

	// get the keystates
	keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_UP])
	    blit_surface((SCR_W - up->w)/2, (SCR_H - up->h)/2,up,scr);
	if (keystate[SDLK_DOWN])
	    blit_surface((SCR_W - down->w)/2, (SCR_H - down->h)/2,down,scr);
	if (keystate[SDLK_LEFT])
	    blit_surface((SCR_W - left->w)/2, (SCR_H - left->h)/2,left,scr);
	if (keystate[SDLK_RIGHT])
	    blit_surface((SCR_W - right->w)/2, (SCR_H - right->h)/2,right,scr);

	if (SDL_Flip(scr) == -1) {
	    fprintf(stderr, "SDL_Flip failed!: %s\n", SDL_GetError());
	    return 1;
	}
    }

    clean_up();
    return 0;
}

SDL_Surface *load_img(char *filename) {
    SDL_Surface *img = NULL;
    if ((img = IMG_Load(filename)) != NULL)
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
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	return false;
    if (TTF_Init() == -1)
	return false;

    scr = SDL_SetVideoMode(SCR_W, SCR_H, SCR_BPP, SDL_SWSURFACE);
    if (scr == NULL)
	return false;

    SDL_WM_SetCaption("Key states", NULL);
    return true;
}

bool load_files() {
    if ((bg = load_img("background.png")) == NULL)
	return false;
    if ((font = TTF_OpenFont("lazy.ttf", 48)) == NULL)
	return false;
    return true;
}

void clean_up() {
    SDL_FreeSurface(bg);
    SDL_FreeSurface(up);
    SDL_FreeSurface(down);
    SDL_FreeSurface(left);
    SDL_FreeSurface(right);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}
