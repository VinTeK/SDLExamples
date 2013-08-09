CC=gcc
CFLAGS=-Wall -O -g -lSDL

adv_timer: adv_timer.c timer.c
	$(CC) adv_timer.c timer.c -o $@ $(CFLAGS) -lSDL_image -lSDL_ttf

clean:
	rm adv_timer
