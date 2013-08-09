#ifndef TIMER_H
#define TIMER_H

typedef enum {false, true} bool;

typedef struct {
    int start_ticks;    // clock time when the timer started
    int paused_ticks;   // ticks stored when timer was paused
    bool started;	// timer started flag
    bool paused;	// timer paused flag
} Timer;

// timer.c functions
Timer new_timer();
void start_timer(Timer *);
void stop_timer(Timer *);
void pause_timer(Timer *);
void unpause_timer(Timer *);
int get_ticks(Timer *);

#endif
