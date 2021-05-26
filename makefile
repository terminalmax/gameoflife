build:
	gcc -std=c99 -o gameoflife main.c `sdl2-config --cflags --libs`

clean:
	rm gameoflife
