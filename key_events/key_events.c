#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

typedef enum {false, true} bool;

const int SCR_W = 640;
const int SCR_H = 480;
const int SCR_BPP = 32;

SDL_Surface *scr = NULL;
SDL_Surface *bg = NULL;
// actual message to blit to screen
SDL_Surface *msg = NULL;
// text message to render
SDL_Surface *up_msg = NULL;
SDL_Surface *down_msg = NULL;
SDL_Surface *left_msg = NULL;
SDL_Surface *right_msg = NULL;

SDL_Event event;

TTF_Font *font = NULL;
SDL_Color font_color = {0x0, 0x0, 0x0};

bool init();
bool load_files();
bool gen_messages();
void clean_up();
SDL_Surface *load_image(char *);
void apply_surface(int, int, SDL_Surface *, SDL_Surface *);

int main(int argc, char *argv[])
{
    bool quit = false;

    if (!init() || !load_files() || !gen_messages())
	return 1;

    // blit the background to the screen
    apply_surface(0, 0, bg, scr);

    while (!quit) {
	if (SDL_PollEvent(&event)) {
	    // if there was a key pressed down
	    if (event.type == SDL_KEYDOWN) {
		// set the respective msg surface
		switch (event.key.keysym.sym) {
		    case SDLK_UP: msg = up_msg; break;
		    case SDLK_DOWN: msg = down_msg; break;
		    case SDLK_LEFT: msg = left_msg; break;
		    case SDLK_RIGHT: msg = right_msg; break;
		    default:;
		}
	    // user Xed out of the window
	    } else if (event.type == SDL_QUIT)
		quit = true;
	    
	    /* If msg isn't pointing to anything, it'll be NULL and nothing will be
	     * blitted. msg should only point to something if there was a key
	     * hit. */
	    if (msg != NULL) {
		// must redraw bg because msg is blitted onto surface
		apply_surface(0, 0, bg, scr);
		apply_surface((SCR_W-msg->w) / 2, (SCR_H-msg->h) / 2, msg, scr);
		// reset msg pointer to NULL
		msg = NULL;
	    }
	    // update the screen
	    if (SDL_Flip(scr) == -1) {
		fprintf(stderr, "SDL_Flip failed!: %s\n", SDL_GetError());
		return 1;
	    }
	}
    }
    clean_up();
    return 0;
}

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
	fprintf(stderr, "SDL_Init failed!: %s\n", SDL_GetError());
	return false;
    }

    scr = SDL_SetVideoMode(SCR_W, SCR_H, SCR_BPP, SDL_SWSURFACE);
    if (scr == NULL) {
	fprintf(stderr, "SDL_SetVideoMode failed!: %s\n", SDL_GetError());
	return false;
    }

    if (TTF_Init() == -1) {
	fprintf(stderr, "TTF_Init failed!: %s\n", TTF_GetError());
	return false;
    }
    
    font = TTF_OpenFont("lazy.ttf", 72);
    if (font == NULL) {
	fprintf(stderr, "TTF_OpenFont failed!: %s\n", TTF_GetError());
	return false;
    }
    SDL_WM_SetCaption("Key press detection", NULL);
    return true;
}

bool load_files() {
    bg = load_image("background.png");
    if (bg == NULL) {
	fprintf(stderr, "load_image failed to load \"background.png\"!\n");
	return false;
    }
    return true;
}

bool gen_messages() {
    up_msg = TTF_RenderText_Solid(font,"Up was pressed.",font_color);
    down_msg = TTF_RenderText_Solid(font,"Down was pressed.",font_color);
    left_msg = TTF_RenderText_Solid(font,"Left was pressed.",font_color);
    right_msg = TTF_RenderText_Solid(font,"Right was pressed.",font_color);
    return true;
}

void clean_up() { 
    SDL_FreeSurface(up_msg);
    SDL_FreeSurface(down_msg);
    SDL_FreeSurface(left_msg);
    SDL_FreeSurface(right_msg);
    SDL_FreeSurface(bg);
    SDL_FreeSurface(msg);

    TTF_CloseFont(font);

    TTF_Quit();
    SDL_Quit();
}

SDL_Surface *load_image(char *filename) {
    SDL_Surface *loaded_img = IMG_Load(filename);
    SDL_Surface *optimized_img = NULL;

    if (loaded_img != NULL) {
	optimized_img = SDL_DisplayFormat(loaded_img);
	SDL_FreeSurface(loaded_img);
    } else
	fprintf(stderr, "SDL_DisplayFormat failed!: %s\n", SDL_GetError());

    return optimized_img;
}

void apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dst) {
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(src, NULL, dst, &offset);
}
