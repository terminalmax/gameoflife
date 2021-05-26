build:
	gcc -o gameoflife main.c `sdl2-config --cflags --libs`

clean:
	rm gameoflife
