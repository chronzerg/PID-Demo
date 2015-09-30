#include "pid.h"

float calculatePIDResponse (struct pidGains gains, float setPoint, float point, float dt) {
	static float prevError = 0;
	static float integral = 0;
	float error;
	float derivative;

	error = setPoint - point;
	integral = integral + (error * dt);
	derivative = (error - prevError) / dt;

	prevError = error;

	return (gains.p * error) + (gains.i * integral) + (gains.d * derivative);
}