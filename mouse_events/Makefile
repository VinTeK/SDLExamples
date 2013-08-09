CC=gcc
CFLAGS=-Wall -O -lSDL

mouse_events: mouse_events.c button.c
	$(CC) mouse_events.c button.c -o $@ $(CFLAGS) -lSDL_image

clean:
	rm mouse_events
