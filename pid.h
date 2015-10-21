struct pidGains {
	float p;
	float i;
	float d;
};

struct pidState {
	float error;
	float integral;
};

float calculatePIDResponse (struct pidState*, struct pidGains, float, float, float);