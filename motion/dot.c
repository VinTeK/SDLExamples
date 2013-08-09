#include "dot.h"
#include "motion.h"

Dot new_dot() {
    Dot d;
    d.x = 0;
    d.y = 0;
    d.x_vel = 0;
    d.y_vel = 0;

    return d;
}

void handle_input(Dot *d, SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
	// add velocity
	switch (event.key.keysym.sym) {
	    case SDLK_UP:   d->y_vel -= DOT_H / 2; break;
	    case SDLK_DOWN: d->y_vel += DOT_H / 2; break;
	    case SDLK_LEFT: d->x_vel -= DOT_W / 2; break;
	    case SDLK_RIGHT:d->x_vel += DOT_W / 2; break;
	}
    } else if (event.type == SDL_KEYUP) {
	// remove velocity
	switch (event.key.keysym.sym) {
	    case SDLK_UP:   d->y_vel += DOT_H / 2; break;
	    case SDLK_DOWN: d->y_vel -= DOT_H / 2; break;
	    case SDLK_LEFT: d->x_vel += DOT_W / 2; break;
	    case SDLK_RIGHT:d->x_vel -= DOT_W / 2; break;
	}
    }
}

void move(Dot *d) {
    // move the dot left/right
    d->x += d->x_vel;
    // check dot bounds on x coord
    if (d->x < 0)
	d->x = 0;
    else if (d->x+DOT_W > SCR_W)
	d->x = d->x+DOT_W + SCR_W;

    // move the dot up/down
    d->y += d->y_vel;
    // check dot bounds on y coord
    if (d->y < 0)
	d->y = 0;
    else if (d->y+DOT_H > SCR_H)
	d->y = d->y+DOT_H + SCR_H;
}

void show(Dot *d) {

}
