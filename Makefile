run:
	cc -o cube cube.c `sdl2-config --cflags --libs` -lm -Wall -Wextra -g
