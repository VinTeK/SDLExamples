CC=gcc
CFLAGS=-Wall -O -g

key_states: key_states.c
	$(CC) key_states.c -o $@ $(CFLAGS) -lSDL -lSDL_image -lSDL_ttf

clean:
	rm key_states
