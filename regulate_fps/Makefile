CC=gcc
CFLAGS=-Wall -O -g -lSDL

regulate_fps: regulate_fps.c timer.h timer.c
	$(CC) regulate_fps.c timer.h timer.c -o $@ $(CFLAGS) -lSDL_image -lSDL_ttf

clean:
	rm regulate_fps
