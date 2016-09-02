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
	void setBlink(int blink);
	int getX() const;
	int getY() const;
	int getBlink() const;
	void saveValues();
	bool hasChanged();

	private:
	int calibrateAxis(int axis);
	int calibrateBlink(int blink);
	int x;
	int y;
	int blink;
	int previousX;
	int previousY;
	int previousBlink;
};

#endif
