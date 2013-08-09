CC=gcc
CFLAGS=-Wall -O -g -lSDL

motion: motion.c dot.h dot.c
	$(CC) motion.c dot.h dot.c -o $@ $(CFLAGS)

clean:
	rm motion
