struct physicalFrame {
	float x;
	float v;
	float a;
};

struct physicalFrame calculateNextPhysicalFrame (struct physicalFrame, float, float, float);