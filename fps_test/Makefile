CC=gcc
CFLAGS=-Wall -O -g -lSDL

fps_test: fps_test.c timer.h timer.c
	$(CC) fps_test.c timer.h timer.c -o $@ $(CFLAGS) -lSDL_image -lSDL_ttf

clean:
	rm fps_test
