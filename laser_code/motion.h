#ifndef motion
#define motion

#include <Arduino.h>
#include "_init.h"
#include "osc.h"

class Motion_C {
	public:
	Motion_C();
	void update(Osc_C osc_);
	void setX(int x);
	void setY(int y);
	void setSpeed(int speed);
	int getX() const;
	int getY() const;
	int getSpeed() const;
	void saveValues();
	bool hasChanged();

	private:
	int calibrateAxis(int axis);
	int calibrateSpeed(int speed);
	int x;
	int y;
	int speed;
	int previousX;
	int previousY;
	int previousSpeed;
};

#endif
