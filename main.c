#include <SDL/SDL.h>
#include "pid.h"
#include "physics.h"

bool running = false;
int setPoint = 0;
float force = 0;
float mass = 1;
int dt = 0;
SDL_Surface* display = NULL;

struct pidGains gains;
struct physicalFrame pFrame;

bool Init () {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

	if ((display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		return false;
	}

	gains.p = 1;
	gains.i = 1;
	gains.d = 1;

	pFrame.x = 0;
	pFrame.v = 0;
	pFrame.a = 0;
}

void Loop () {
	// TODO - Calculate dt.

	pFrame = calculateNextPhysicalFrame(pFrame, mass, force, dt);
	force = calculatePIDResponse(gains, setPoint, pFrame.x, dt);
}

void Render () {
	// TODO - Render the frame.
}

void CleanUp () {
	SDL_FreeSurface(display);
	SDL_Quit();
}

void OnEvent (SDL_Event* event) {
	if (event->type == SDL_MOUSEMOTION) {
		setPoint = event->motion.x;
	}
}

int main (int argc, char *argv[]) {
	if (Init() == false) {
		return -1;
	}

	SDL_Event event;

	while(running) {
		while(SDL_PollEvent(&event)) {
			OnEvent(&event);
		}

		Loop();
		Render();
	}

	CleanUp();

	return 0;
}