#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

typedef struct {
    // the attributes of the button
    Sint16 x, y;
    Uint16 w, h;
    // the part of the button sprite sheet that will be shown
    SDL_Rect *clip;
} Button;

typedef enum {false, true} bool;
enum {CLIP_MOUSEOVER, CLIP_MOUSEOUT, CLIP_MOUSEDOWN, CLIP_MOUSEUP};

const int SCR_W = 640;
const int SCR_H = 480;
const int SCR_BPP = 32;

SDL_Surface *scr = NULL;
SDL_Surface *button_sheet = NULL;
SDL_Rect clips[4];
SDL_Event event;

// defined in button.c
void handle_events(Button);
void show(Button);
// fuctions for mouse_events.c
SDL_Surface *load_image(char * );
void blit_surface(int, int, SDL_Surface *, SDL_Surface * , SDL_Rect * );
bool init();
bool load_files();
void set_clips();
void clean_up();

int main(int argc, char *argv[]) {
    bool quit = false;

    if (!init() || !load_files())
	return 1;

    set_clips();

    // create and set the button's attributes
    Button button;
    button.x = 170;
    button.y = 120;
    button.w = 320;
    button.h = 240;
    button.clip = &clips[ CLIP_MOUSEOUT ];

    while (!quit) {
	if (SDL_PollEvent(&event)) {
	    // handle button events
	    handle_events(button);

	    if (event.type == SDL_QUIT)
		quit = true;
	}

	SDL_FillRect(scr,&scr->clip_rect,SDL_MapRGB(scr->format,255,255,255));
	show(button);
	if (SDL_Flip(scr) == -1) {
	    fprintf(stderr, "SDL_Flip failed!: %s\n", SDL_GetError());
	    return 1;
	}
    }


    return 0;
}

SDL_Surface *load_image(char *filename) {
    SDL_Surface *img = NULL;
    if ((img = IMG_Load(filename)) != NULL)
	img = SDL_DisplayFormat(img);
    return img;
}

void blit_surface(int x,int y,SDL_Surface *src,SDL_Surface *dst,SDL_Rect *clip) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(src, clip, dst, &offset);
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

    SDL_WM_SetCaption("Mouse events", NULL);
    return true;
}

bool load_files() {
    button_sheet = load_image("button.png");
    if (button_sheet == NULL) {
	fprintf(stderr, "load_image failed to load \"button.png\"!\n");
	return false;
    }
    return true;
}

void set_clips() {
    // clip the sprite sheet
    int i;
    for (i = 0; i < 4; ++i) {
	clips[i].w = 320;
	clips[i].h = 240;
    } 
    clips[ CLIP_MOUSEOVER ].x = 0;
    clips[ CLIP_MOUSEOVER ].y = 0;

    clips[ CLIP_MOUSEOUT ].x = 320;
    clips[ CLIP_MOUSEOUT ].y = 0;

    clips[ CLIP_MOUSEDOWN ].x = 0;
    clips[ CLIP_MOUSEDOWN ].y = 240;

    clips[ CLIP_MOUSEUP ].x = 320;
    clips[ CLIP_MOUSEUP ].y = 240;
}

void clean_up() {
    SDL_FreeSurface(button_sheet);
    SDL_Quit();
}
