#ifndef sonars
#define sonars

#include <Arduino.h>
#include <NewPing.h>

#include "_init.h"
#include "motion.h"

typedef struct Sonar {
	NewPing sensor;
	const int ANGLE;
	int value;
	const int TRIGGER_PIN;
	const int ECHO_PIN;
};


class Sonars_C {
	public:
	Sonars_C();
	void init();
	void correct(Motion_C& motion_);

	private:
	void read();
	void defineDistanceThreshold(Motion_C& motion_);
	void detectObjects();
	void chooseDirection(Motion_C& motion_);
	void reset(int i);
	Sonar sonars_[10];
	int distanceThreshold;
	bool isDetectingObjects;
	int objectAngle;
	unsigned long sensorsCheckTimeout;
};

#endif

// Read => Determine objects => Choose direction => Update motion