#include "pid.h"

float calculatePIDResponse (struct pidState* state, struct pidGains gains, float setPoint, float point, float dt) {
	float currentError;

	currentError = setPoint - point;
	state->integral = state->integral + (currentError * dt);
	state->derivative = (currentError - state->error) / dt;

	state->error = currentError;

	return (gains.p * state->error) + (gains.i * state->integral) + (gains.d * state->derivative);
}