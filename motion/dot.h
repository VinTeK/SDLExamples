#ifndef DOT_H
#define DOT_H

#include "SDL/SDL.h"

const int DOT_W = 20;
const int DOT_H = 20;

typedef enum {false, true} bool;

typedef struct {
    int x, y;		// the x and y offsets of the dot
    int x_vel, y_vel;	// the velocities of the dot
} Dot;

// create and initialize new Dot
Dot new_dot();
// takes key presses and adjusts the dots velocity
void handle_input(Dot *);
// moves the dot
void move(Dot *);
// shows the dot on screen
void show(Dot *);

#endif
