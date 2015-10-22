#include <SDL.h>
#include <SDL_timer.h>
#include <stdio.h>
#include <math.h>
#include "pid.h"
#include "physics.h"

#define TRUE 0
#define FALSE 1

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define POLICE_WIDTH 70
#define POLICE_HEIGHT 35
#define POLICE_MASS 1

int running = TRUE;
int mouseX = 0;
int mouseY = 0;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* police = NULL;
SDL_Rect policeDest;

struct pidGains gains;
struct pidState stateX;
struct pidState stateY;

struct physicalFrame frameX;
struct physicalFrame frameY;

int Init () {
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        return 1;
    }

	if ((window = SDL_CreateWindow("PID Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN)) == NULL) {
		return 2;
	}

	if ((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL) {
		return 3;
	}

	SDL_Surface* policeBMP;
	if ((policeBMP = SDL_LoadBMP("./police.bmp")) == NULL) {
		return 4;
	}

	if ((police = SDL_CreateTextureFromSurface(renderer, policeBMP)) == NULL) {
		return 5;
	}

	SDL_FreeSurface(policeBMP);

	SDL_Cursor* cursor;
	if ((cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR)) == NULL) {
		return 6;
	}

	SDL_SetCursor(cursor);

	policeDest.w = POLICE_WIDTH;
	policeDest.h = POLICE_HEIGHT;

	gains.p = 10;
	gains.i = 1;
	gains.d = 5;

	frameX.p = (SCREEN_WIDTH / 2) - (POLICE_WIDTH / 2);
	frameX.v = 0;
	frameX.a = 0;

	frameY.p = (SCREEN_HEIGHT / 2) - (POLICE_HEIGHT / 2);
	frameY.v = 0;
	frameY.a = 0;

	stateX.error = 0;
	stateX.integral = 0;

	stateY.error = 0;
	stateY.integral = 0;

	return 0;
}

void Loop () {
	static int lastTicks = 0;
	static float forceX = 0;
	static float forceY = 0;
	float dt;
	int ticks;

	ticks = SDL_GetTicks();
	dt = (ticks - lastTicks) / 1000.0;
	lastTicks = ticks;

	if (dt == 0) {
		return;
	}

	frameX = calculateNextPhysicalFrame(frameX, POLICE_MASS, forceX, dt);
	frameY = calculateNextPhysicalFrame(frameY, POLICE_MASS, forceY, dt);

	forceX = calculatePIDResponse(&stateX, gains, mouseX, frameX.p, dt);
	forceY = calculatePIDResponse(&stateY, gains, mouseY, frameY.p, dt);
}

void Render () {
	policeDest.x = frameX.p - (POLICE_WIDTH / 2);
	policeDest.y = frameY.p - (POLICE_HEIGHT / 2);

	float angle = atan2(-frameY.v, -frameX.v) * (180.0 / M_PI);

	// Clear frame to black
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Draw police car
	SDL_RenderCopyEx(renderer, police, NULL, &policeDest, angle, NULL, SDL_FLIP_NONE);

	// Draw vectors
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, frameX.p, frameY.p, frameX.p+(frameX.a/5), frameY.p+(frameY.a/5));
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderDrawLine(renderer, frameX.p, frameY.p, frameX.p+(frameX.v/5), frameY.p+(frameY.v/5));

	// Flip frame
	SDL_RenderPresent(renderer);
}

void OnEvent (SDL_Event* event) {
	switch(event->type) {
		case SDL_MOUSEMOTION:
			mouseX = event->motion.x;
			mouseY = event->motion.y;
			break;

		case SDL_KEYUP:
			if (event->key.keysym.sym == SDLK_q) {
				running = FALSE;
			}
			break;

		case SDL_QUIT:
			running = FALSE;
			break;
	}
}

void PrintError (int error) {
	switch(error) {
		case 1:
			puts("Failed to init SDL.");
			break;
		case 2:
			puts("Failed to create SDL window.");
			break;
		case 3:
			puts("Failed to creater SDL renderer.");
			break;
		case 4:
			puts("Failed to load police.bmp.");
			break;
		case 5:
			puts("Failed to texturize police.png.");
			break;
		case 6:
			puts("Failed to create cursor.");
	}
}

int main (int argc, char *argv[]) {
	SDL_Event event;

	int error = Init();
	if (error) {
		PrintError(error);
		SDL_Quit();
		return -1;
	}

	while(running == TRUE) {
		while(SDL_PollEvent(&event)) { OnEvent(&event); }
		Loop();
		Render();
	}

	SDL_Quit();
	return 0;
}