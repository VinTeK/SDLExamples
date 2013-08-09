CC=gcc
CFLAGS=-Wall -O -lSDL

key_events: key_events.c
	$(CC) key_events.c -o $@ $(CFLAGS) -lSDL_image -lSDL_ttf

clean:
	rm key_events
