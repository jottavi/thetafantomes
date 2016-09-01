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
	// Is the incoming message matching the right IP ?
	if (osc_.getIp() == PLAYER_IP || osc_.getIp() == MASTER_IP) {
		// Then grab the value and update the speed or the angle

		String route = osc_.getRoute();
		int value = osc_.getValue();

		if (route == "/speed") {
			this->setSpeed(value);
		} else if (route == "/angle") {
			this->setAngle(value);
		}

		// Then just trash the message
		osc_.clean();
	}

	// In any case, line the speed to avoid brutal changes for the motors
	this->lineSpeed();
}


void Motion_C::setSpeed(int speed) {
	// Calibrate the speed and update it
	// The polarity is either 1 or -1, and is switched when an object is detected
	speed = this->calibrateSpeed(speed) * this->polarity;
	this->reachingSpeed = speed;
}


void Motion_C::setAngle(int angle) {
	// Calibrate the angle and update it
	// The polarity is either 1 or -1, and is switched when an object is detected
	angle = this->calibrateAngle(angle) * this->polarity;
	this->angle = angle;
}


void Motion_C::switchPolarity() {
	// The polarity is either 1 or -1, and is switched when an object is detected
	this->polarity = -this->polarity;
}


int Motion_C::calibrateSpeed(int speed) {
	// floor any eventual overloading speed values
	if (speed > MAX_SPEED) {
		speed = MAX_SPEED;
	} else if (speed < -MAX_SPEED) {
		speed = -MAX_SPEED;
	}

	// bring speed to 0 when the value is under 31 (= tone limit : https://www.arduino.cc/en/Reference/Tone)
	if (abs(speed) < 31) {
		speed = 0;
	}

	return speed;
}


int Motion_C::calibrateAngle(int angle) {
	// floor any eventual overloading angle values
	if (angle > 180) {
		angle = 180;
	}
	else if (angle < -180) {
		angle = -180;
	}

	return angle;
}


void Motion_C::lineSpeed() {
	// If it's time to line the speed when full speed is not reached yet, do it
	if (millis() > this->time + this->lineTimer && this->speed != this->reachingSpeed) {
		// When the difference between the reaching speed and the actual speed is big, reach it fast
		// else, do it slowly
		if (this->speed < this->reachingSpeed) { // When speed is going forwards
			if (abs(this->reachingSpeed - this->speed) > 20) {
				this->speed += 8;
			} else {
				this->speed += 1;
			}
		} else if (this->speed > this->reachingSpeed) { // When speed is going backwards
			if (abs(this->reachingSpeed - this->speed) > 20) {
				this->speed -= 8;
			} else {
				this->speed -= 1;
			}
		}

		// And don't forget what time it was...
		// ...For the next round
		this->time = millis();
	}
}


void Motion_C::saveValues() {
	// Save previous values to be aware of the future changes
	this->previousSpeed = this->speed;
	this->previousAngle = this->angle;
}


bool Motion_C::hasChanged() {
	// Say yes if the motion has changed compared to the previous one, or say no.
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
