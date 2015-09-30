all:
	gcc -o demo main.c physics.c pid.c -I/usr/local/include/SDL2 -I/usr/X11R6/include -D_THREAD_SAFE -L/usr/local/lib -lSDL2


