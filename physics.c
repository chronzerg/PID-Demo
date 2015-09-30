#include "physics.h"

struct physicalFrame calculateNextPhysicalFrame (struct physicalFrame frame, float mass, float force, float dt) {
	float a = force / mass;
	float v = (a * dt) + frame.v;
	float x = ((a * dt * dt) / 2) + (v * dt) + frame.x;

	struct physicalFrame nextFrame;
	nextFrame.x = x;
	nextFrame.v = v;
	nextFrame.a = a;

	return physicalFrame;
}