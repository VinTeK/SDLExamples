#include "SDL/SDL.h"

typedef enum {false, true} bool;

typedef struct {
    int start_ticks;    // clock time when the timer started
    int paused_ticks;   // ticks stored when timer was paused
    bool started;	// timer started flag
    bool paused;	// timer paused flag
} Timer;

Timer new_timer() {
    Timer t;
    t.start_ticks = 0;
    t.paused_ticks = 0;
    t.started = false;
    t.paused = false;
    return t;
}

void start_timer(Timer *t) {
    // start the timer
    t->started = true;
    // unpause the timer
    t->paused = false;
    // get current clock time
    t->start_ticks = SDL_GetTicks();
}

void stop_timer(Timer *t) {
    t->started = false;
    t->paused = false;
    t->start_ticks = 0;
}

void pause_timer(Timer *t) {
    // if timer is already running and isn't already paused
    if (t->started == true && !t->paused) {
	// calculate the paused ticks
	t->paused_ticks = SDL_GetTicks() - t->start_ticks;
	t->paused = true;
    }
}

void unpause_timer(Timer *t) {
    // if the timer is already running and is paused
    if (t->started == true && t->paused) {
	// reset the starting ticks
	t->start_ticks = SDL_GetTicks() - t->paused_ticks;
	t->paused_ticks = 0;
	t->paused = false;
    }
}

int get_ticks(Timer *t) {
    // if timer is started
    if (t->started) {
	if (t->paused)
	    // return the ticks when the timer was paused
	    return t->paused_ticks;
	else
	    // return the current time minus the start time
	    return SDL_GetTicks() - t->start_ticks;
    }
    // if the timer is stopped
    return 0;
}
