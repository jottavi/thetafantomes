#ifndef laser
#define laser

#include <Arduino.h>
#include <DmxSimple.h>
#include "_init.h"
#include "motion.h"

typedef struct LaserChannels {
	const int SETUP;
	const int X;
	const int Y;
	const int SPEED;
};

class Laser_C {
	public:
	Laser_C(int deviceId, String playerIp);
	void move(Motion_C& motion_);
	void init();
	String getIp() const;

	private:
	void update(Motion_C motion_);
	LaserChannels CHANNELS;
	String ip;
};

#endif
