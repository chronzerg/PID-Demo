struct physicalFrame {
	float p;
	float v;
	float a;
};

struct physicalFrame calculateNextPhysicalFrame (struct physicalFrame, float, float, float);