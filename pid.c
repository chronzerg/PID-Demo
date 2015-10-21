#include "pid.h"

float calculatePIDResponse (struct pidState* state, struct pidGains gains, float setPoint, float point, float dt) {
	float error;
	float derivative;

	error = setPoint - point;
	state->integral = state->integral + (error * dt);
	derivative = (error - state->error) / dt;

	state->error = error;

	return (gains.p * error) + (gains.i * state->integral) + (gains.d * derivative);
}