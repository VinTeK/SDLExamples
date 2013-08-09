#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

typedef struct {
    // the attributes of the button
    Sint16 x, y;
    Uint16 w, h;
    // the part of the button sprite sheet that will be shown
    SDL_Rect *clip;
} Button;

//typedef enum {false, true} bool;
enum {CLIP_MOUSEOVER, CLIP_MOUSEOUT, CLIP_MOUSEDOWN, CLIP_MOUSEUP};

// defined in mouse_events.c
extern SDL_Surface *scr, *button_sheet;
extern SDL_Rect clips[];
extern SDL_Event event;

// defined in mouse_events.c
void blit_surface(int, int, SDL_Surface *, SDL_Surface *, SDL_Rect *);
// functions for button.c
void handle_events(Button);
void show(Button);

void handle_events(Button b) {
    // the mouse offsets
    int x = 0, y = 0;

    // if the mouse moved
    if (event.type == SDL_MOUSEMOTION) {
	// get the mouse offsets
	x = event.motion.x;
	y = event.motion.y;

	// if the mouse is over the button
	if ((x > b.x) && (x < b.x + b.w) && (y > b.y) && (y < b.y + b.h)) {
	    // the button sprite
	    b.clip = &clips[ CLIP_MOUSEOVER ];
	} else {
	    // set the button sprite
	    b.clip = &clips[ CLIP_MOUSEOUT ];
	}
	//printf("MOUSE MOVED\t(%d, %d)\n", x, y);
    }
    // if a mouse button was pressed
    if (event.type == SDL_MOUSEBUTTONDOWN) {
	// if the left mouse button was pressed
	if (event.button.button == SDL_BUTTON_LEFT) {
	    // get the mouse offsets
	    x = event.button.x;
	    y = event.button.y;

	    // if the mouse if over the button
	    if ((x > b.x) && (x < b.x + b.w) && (y > b.y) && (y < b.y + b.h)) {
		// set the button sprite
		b.clip = &clips[ CLIP_MOUSEDOWN ];
	    }
	}
	//printf("MOUSE DOWN\t(%d, %d)\n", x, y);
    }
    // if a mouse button was released
    if (event.type == SDL_MOUSEBUTTONUP) {
	// if the left mouse button was released
	if (event.button.button == SDL_BUTTON_LEFT) {
	    // get the mouse offsets
	    x = event.button.x;
	    y = event.button.y;

	    // if the mouse is over the button
	    if ((x > b.x) && (x < b.x + b.w) && (y > b.y) && (y < b.y + b.h)) {
		// set the button sprite
		b.clip = &clips[ CLIP_MOUSEUP ];
	    }
	}
	//printf("MOUSE UP\t(%d, %d)\n", x, y);
    }
}

void show(Button b) {
    // show the button
    blit_surface(b.x, b.y, button_sheet, scr, b.clip);
}
