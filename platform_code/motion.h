#ifndef motion
#define motion

#include <Arduino.h>

#include "_init.h"
#include "osc.h"

class Motion_C {
	public:
	Motion_C();
	void update(Osc_C osc_);
	bool hasChanged();
	int getSpeed() const;
	int getAngle() const;
	int getReachingSpeed() const;
	void saveValues();
	void switchPolarity();

	private:
	int calibrateSpeed(int speed);
	int calibrateAngle(int angle);
	void setSpeed(int speed);
	void setAngle(int angle);
	void lineSpeed();
	long time;
	int lineTimer;
	int reachingSpeed;
	int speed;
	int angle;
	int previousSpeed;
	int previousAngle;
	int polarity;
};

#endif
