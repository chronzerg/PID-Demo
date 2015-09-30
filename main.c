#include <SDL.h>
#include <SDL_timer.h>
#include <stdio.h>
#include "pid.h"
#include "physics.h"

#define TRUE 0
#define FALSE 1

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define MARKER_WIDTH 60
#define MARKER_HEIGHT 20
#define MARKER_MARGIN 50
#define MARKER_MASS 1

int running = TRUE;
int mouseX = 0;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

struct pidGains gains;
struct physicalFrame pFrame;

int Init () {
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        return FALSE;
    }

	if ((window = SDL_CreateWindow("PID Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0)) == NULL) {
		return FALSE;
	}

	if ((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL) {
		return FALSE;
	}

	gains.p = 20;
	gains.i = 1;
	gains.d = 5;

	pFrame.x = (SCREEN_WIDTH / 2) - (MARKER_WIDTH / 2);
	pFrame.v = 0;
	pFrame.a = 0;

	return TRUE;
}

void Loop () {
	static int lastTicks = 0;
	static float force = 0;
	float dt;
	int ticks;

	ticks = SDL_GetTicks();
	dt = (ticks - lastTicks) / 1000.0;
	lastTicks = ticks;

	if (dt == 0) {
		return;
	}

	pFrame = calculateNextPhysicalFrame(pFrame, MARKER_MASS, force, dt);
	force = (calculatePIDResponse(gains, mouseX, pFrame.x, dt));

	printf("%d | %f %f %f | %f | %f\n", mouseX, pFrame.x, pFrame.v, pFrame.a, force, dt);
}

void Render () {
	struct SDL_Rect rect;
	rect.x = pFrame.x - (MARKER_WIDTH / 2);
	rect.y = MARKER_MARGIN;
	rect.w = MARKER_WIDTH;
	rect.h = MARKER_HEIGHT;

	// Clear frame to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Draw white rectangle
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);

	// Flip frame
	SDL_RenderPresent(renderer);
}

void OnEvent (SDL_Event* event) {
	switch(event->type) {
		case SDL_MOUSEMOTION:
			mouseX = event->motion.x;
			break;

		case SDL_QUIT:
			running = FALSE;
			break;
	}
}

void CleanUp () {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	printf("Quiting...\n");
}

int main (int argc, char *argv[]) {
	SDL_Event event;

	if (Init() == FALSE) {
		return -1;
	}

	while(running == TRUE) {
		while(SDL_PollEvent(&event)) {
			OnEvent(&event);
		}

		Loop();
		Render();
	}

	CleanUp();

	return 0;
}