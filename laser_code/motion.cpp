#include "motion.h"

Motion_C::Motion_C() :
	x(0),
	y(0),
	speed(0),
	previousX(0),
	previousY(0),
	previousSpeed(0)
{}


void Motion_C::update(Osc_C osc_) {
	// Grab the value and update x or y
	String route = osc_.getRoute();
	int value = osc_.getValue();

	if (route == "/x") {
		this->setX(value);
	} else if (route == "/y") {
		this->setY(value);
	} else if (route == "/speed") {
		this->setSpeed(value);
	}
}


void Motion_C::setX(int x) {
	// Calibrate the x position and update it
	x = this->calibrateAxis(x);
	this->x = x;
}


void Motion_C::setY(int y) {
	// Calibrate the y position and update it
	y = this->calibrateAxis(y);
	this->y = y;
}


void Motion_C::setSpeed(int speed) {
	// Calibrate the speed and update it
	speed = this->calibrateSpeed(speed);
	this->speed = speed;
}


int Motion_C::calibrateAxis(int axis) {
	// floor eventual overloading axis values
	if (axis > 127) {
		axis = 127;
	} else if (axis < 0) {
		axis = 0;
	}

	return axis;
}


int Motion_C::calibrateSpeed(int speed) {
	// floor eventual overloading speed values
	if (speed > 170) {
		speed = 170;
	}
	else if (speed < 0) {
		speed = 0;
	}

	// The value should start at 26 (see the Fat Beamer documentation)
	return speed + 26;
}


int Motion_C::getX() const {
	return this->x;
}


int Motion_C::getY() const {
	return this->y;
}


int Motion_C::getSpeed() const {
	return this->speed;
}


void Motion_C::saveValues() {
	// Save previous values to be aware of the future changes
	this->previousX = this->x;
	this->previousY = this->y;
	this->previousSpeed = this->speed;
}


bool Motion_C::hasChanged() {
	// Say yes if the motion has changed compared to the previous one, or say no.
	if (this->x != this->previousX || this->y != this->previousY || this->speed != this->previousSpeed) {
		return true;
	} else {
		return false;
	}
}
