SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs)

all:
	gcc -o demo main.c physics.c pid.c $(SDL_CFLAGS) $(SDL_LIBS) -lSDL2_image