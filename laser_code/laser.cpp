#include "laser.h"

Laser_C::Laser_C(int deviceId, String playerIp) :
	CHANNELS {
		DMX_DEVICE_CHANNEL_NB * deviceId + 1,
		DMX_DEVICE_CHANNEL_NB * deviceId + 2,
		DMX_DEVICE_CHANNEL_NB * deviceId + 3,
		DMX_DEVICE_CHANNEL_NB * deviceId + 4,
	},
	ip(playerIp)
{}


void Laser_C::move(Motion_C& motion_) {
	// Change the laser action only if the calculated motions have changed
	if (motion_.hasChanged()) {
		this->update(motion_);

		// Backup the values for the next round
		motion_.saveValues();
	}
}


void Laser_C::update(Motion_C motion_) {
	int x = motion_.getX();
	int y = motion_.getY();
	int speed = motion_.getSpeed();

	// Change the laser's position
	DmxSimple.write(this->CHANNELS.X, x);
	DmxSimple.write(this->CHANNELS.Y, y);
	DmxSimple.write(this->CHANNELS.SPEED, speed);
}


void Laser_C::init() {
	// Initialize the laser DMX channels (see the Fat Beamer documentation)
	DmxSimple.write(this->CHANNELS.SETUP, 193);
	DmxSimple.write(this->CHANNELS.X, 191);
	DmxSimple.write(this->CHANNELS.X, 0);
	DmxSimple.write(this->CHANNELS.Y, 191);
	DmxSimple.write(this->CHANNELS.Y, 0);
	DmxSimple.write(this->CHANNELS.SPEED, 196);
}


String Laser_C::getIp() const {
	return this->ip;
}
