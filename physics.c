#include "physics.h"

struct physicalFrame calculateNextPhysicalFrame (struct physicalFrame frame, float mass, float force, float dt) {
	float a = force / mass;
	float v = (a * dt) + frame.v;
	float p = ((a * dt * dt) / 2) + (v * dt) + frame.p;

	struct physicalFrame nextFrame;
	nextFrame.p = p;
	nextFrame.v = v;
	nextFrame.a = a;

	return nextFrame;
}