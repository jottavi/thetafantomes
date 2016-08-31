#include "motion.h"

Motion_C::Motion_C() :
	speed(0),
	angle(0),
	previousSpeed(0),
	previousAngle(0),
	reachingSpeed(0),
	time(0),
	lineTimer(1),
	polarity(1)
{}


void Motion_C::update(Osc_C osc_) {
	String route = osc_.getRoute();
	int value = osc_.getValue();

	if (route == "/speed") {
		this->setSpeed(value);
	} else if (route == "/angle") {
		this->setAngle(value);
	}

	this->lineSpeed();

	osc_.clean();
}


void Motion_C::setSpeed(int speed) {
	speed = this->calibrateSpeed(speed) * this->polarity;
	this->reachingSpeed = speed;
}


void Motion_C::setAngle(int angle) {
	angle = this->calibrateAngle(angle) * this->polarity;
	this->angle = angle;
}


void Motion_C::switchPolarity() {
	this->polarity = -this->polarity;
}


int Motion_C::calibrateSpeed(int speed) {
	// floor eventual overloading speed values
	if (speed > MAX_SPEED) {
		speed = MAX_SPEED;
	} else if (speed < -MAX_SPEED) {
		speed = -MAX_SPEED;
	}

	// bring speed to 0 when the value is under 31 (= tone limit : https://www.arduino.cc/en/Reference/Tone)
	if (abs(speed) < SPEED_THRESHOLD) {
		speed = 0;
	}

	return speed;
}


int Motion_C::calibrateAngle(int angle) {
	// floor eventual overloading angle values
	if (angle > 180) {
		angle = 180;
	}
	else if (angle < -180) {
		angle = -180;
	}

	return angle;
}


void Motion_C::lineSpeed() {
	if (millis() > this->time + this->lineTimer) {
		if (this->speed < this->reachingSpeed) {
			if (abs(this->reachingSpeed - this->speed) > 20) {
				this->speed += 8;
			} else {
				this->speed += 1;
			}
		} else if (this->speed > this->reachingSpeed) {
			if (abs(this->reachingSpeed - this->speed) > 20) {
				this->speed -= 8;
			} else {
				this->speed -= 1;
			}
		}
		this->time = millis();
	}
}


void Motion_C::saveValues() {
	this->previousSpeed = this->speed;
	this->previousAngle = this->angle;
}


bool Motion_C::hasChanged() {
	if (this->speed != this->previousSpeed || this->angle != this->previousAngle) {
		return true;
	} else {
		return false;
	}
}


int Motion_C::getSpeed() const {
	return this->speed;
}


int Motion_C::getReachingSpeed() const {
	return this->reachingSpeed;
}


int Motion_C::getAngle() const {
	return this->angle;
}
