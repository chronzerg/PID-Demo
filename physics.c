#include "physics.h"

struct physicalFrame calculateNextPhysicalFrame (struct physicalFrame frame, float mass, float force, float dt) {
	struct physicalFrame nextFrame;

	nextFrame.a = force / mass;
	nextFrame.v = (nextFrame.a * dt) + frame.v;
	nextFrame.p = ((nextFrame.a * dt * dt) / 2) + (nextFrame.v * dt) + frame.p;

	return nextFrame;
}