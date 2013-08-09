CC=gcc
CFLAGS=-Wall -O -g -lSDL

play_sounds: play_sounds.c
	$(CC) play_sounds.c -o $@ $(CFLAGS) -lSDL_image -lSDL_ttf -lSDL_mixer

clean:
	rm play_sounds
