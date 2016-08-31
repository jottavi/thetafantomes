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
	if (motion_.hasChanged()) {
		this->update(motion_);
		motion_.saveValues();
	}
}


void Laser_C::update(Motion_C motion_) {
	int x = motion_.getX();
	int y = motion_.getY();
	int speed = motion_.getSpeed();

	DmxSimple.write(this->CHANNELS.X, x);
	DmxSimple.write(this->CHANNELS.Y, y);
}


void Laser_C::init() {
	DmxSimple.write(this->CHANNELS.SETUP, 192);
	DmxSimple.write(this->CHANNELS.X, 191);
	DmxSimple.write(this->CHANNELS.Y, 191);
	DmxSimple.write(this->CHANNELS.SPEED, 196);
}


String Laser_C::getIp() const {
	return this->ip;
}
