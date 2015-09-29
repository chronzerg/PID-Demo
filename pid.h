struct pidGains
{
	float p;
	float i;
	float d;
};

float calculatePIDResponse (struct pidGains, float, float, float);