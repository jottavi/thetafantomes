#include "motors.h"

Motors_C::Motors_C() :
	RIGHT_MOTOR {
		RIGHT_MOTOR_ENABLE_PIN,
		RIGHT_MOTOR_DIRECTION_PIN,
		RIGHT_MOTOR_STEP_PIN,
		RIGHT_MOTOR_MS1_PIN,
		RIGHT_MOTOR_MS2_PIN,
		RIGHT_MOTOR_MS3_PIN
	},
	LEFT_MOTOR {
		LEFT_MOTOR_ENABLE_PIN,
		LEFT_MOTOR_DIRECTION_PIN,
		LEFT_MOTOR_STEP_PIN,
		LEFT_MOTOR_MS1_PIN,
		LEFT_MOTOR_MS2_PIN,
		LEFT_MOTOR_MS3_PIN
	},
	areEnabled(true)
{}


void Motors_C::init() {
	// Set motors pins and their values
	pinMode(this->LEFT_MOTOR.ENABLE_PIN, OUTPUT);
	pinMode(this->LEFT_MOTOR.DIRECTION_PIN, OUTPUT);
	pinMode(this->LEFT_MOTOR.STEP_PIN, OUTPUT);
	pinMode(this->LEFT_MOTOR.MS1_PIN, OUTPUT);
	pinMode(this->LEFT_MOTOR.MS2_PIN, OUTPUT);
	pinMode(this->LEFT_MOTOR.MS3_PIN, OUTPUT);

	pinMode(this->RIGHT_MOTOR.ENABLE_PIN, OUTPUT);
	pinMode(this->RIGHT_MOTOR.DIRECTION_PIN, OUTPUT);
	pinMode(this->RIGHT_MOTOR.STEP_PIN, OUTPUT);
	pinMode(this->RIGHT_MOTOR.MS1_PIN, OUTPUT);
	pinMode(this->RIGHT_MOTOR.MS2_PIN, OUTPUT);
	pinMode(this->RIGHT_MOTOR.MS3_PIN, OUTPUT);

	digitalWrite(this->LEFT_MOTOR.ENABLE_PIN, LOW);
	digitalWrite(this->LEFT_MOTOR.DIRECTION_PIN, FORWARDS);
	digitalWrite(this->LEFT_MOTOR.STEP_PIN, LOW);
	digitalWrite(this->LEFT_MOTOR.MS1_PIN, HIGH);
	digitalWrite(this->LEFT_MOTOR.MS2_PIN, HIGH);
	digitalWrite(this->LEFT_MOTOR.MS3_PIN, HIGH);

	digitalWrite(this->RIGHT_MOTOR.ENABLE_PIN, LOW);
	digitalWrite(this->RIGHT_MOTOR.DIRECTION_PIN, FORWARDS);
	digitalWrite(this->RIGHT_MOTOR.STEP_PIN, LOW);
	digitalWrite(this->RIGHT_MOTOR.MS1_PIN, HIGH);
	digitalWrite(this->RIGHT_MOTOR.MS2_PIN, HIGH);
	digitalWrite(this->RIGHT_MOTOR.MS3_PIN, HIGH);

	// Start the tones on the step pin
	this->LEFT_MOTOR.rate.begin(this->LEFT_MOTOR.STEP_PIN);
	this->RIGHT_MOTOR.rate.begin(this->RIGHT_MOTOR.STEP_PIN);
}


void Motors_C::move(Motion_C& motion_) {
	// Change the motors' action only if the calculated motions have changed
	if (motion_.hasChanged()) {
		this->updateDirection(motion_);
		this->updateSpeed(motion_);

		// Backup the values for the next round
		motion_.saveValues();
	}
}


void Motors_C::updateDirection(Motion_C motion_) { // A little bit tricky, have fun.
	int angle = motion_.getAngle();
	int speed = motion_.getSpeed();

	if (angle >= -90 && angle <= 90 && speed > 0) {
		// When you are going FORWARDS and turning a LITTLE, keep the motors FORWARDS
		digitalWrite(this->RIGHT_MOTOR.DIRECTION_PIN, FORWARDS);
		digitalWrite(this->LEFT_MOTOR.DIRECTION_PIN, FORWARDS);
	} else 	if (angle >= -90 && angle <= 90 && speed < 0) {
		// When you are going BACKWARDS and you are not turing or you are turning a LITTLE, keep the motors FORWARDS
		digitalWrite(this->RIGHT_MOTOR.DIRECTION_PIN, BACKWARDS);
		digitalWrite(this->LEFT_MOTOR.DIRECTION_PIN, BACKWARDS);
	} else if ((angle < -90 && speed > 0) || (angle > 90 && speed < 0)) {
		// When you are going FORWARDS and you are turning a LOT ON THE LEFT, send the left motor BACKWARDS and the right one FORWARDS
		// OR when you are going BACKWARDS and you are turning a LOT ON THE RIGHT, send the left motor BACKWARDS and the right one FORWARDS
		digitalWrite(this->RIGHT_MOTOR.DIRECTION_PIN, FORWARDS);
		digitalWrite(this->LEFT_MOTOR.DIRECTION_PIN, BACKWARDS);
	} else if ((angle > 90 && speed > 0) || (angle < -90 && speed < 0)) {
		// When you are going FORWARDS and you are turning a LOT ON THE RIGHT, send the RIGHT motor FORWARDS and the right one BACKWARDS
		// OR when you are going BACKWARDS and you are turning a LOT ON THE LEFT, send the LEFT motor FORWARDS and the right one BACKWARDS
		digitalWrite(this->RIGHT_MOTOR.DIRECTION_PIN, BACKWARDS);
		digitalWrite(this->LEFT_MOTOR.DIRECTION_PIN, FORWARDS);
	}
}


void Motors_C::updateSpeed(Motion_C motion_) {
	int angle = motion_.getAngle();
	int speed = motion_.getSpeed();

	int absAngle = abs(abs(angle) - 90);


	// Re-activate the motors when the speed is kicking
	if (speed != 0 && this->areEnabled == false) {
		this->areEnabled = true;
		digitalWrite(this->RIGHT_MOTOR.ENABLE_PIN, LOW);
		digitalWrite(this->LEFT_MOTOR.ENABLE_PIN, LOW);
	}


	if (angle == 0) {
		// set both motors' speed equaly and go straight when there's no angle value
		this->RIGHT_MOTOR.rate.play(abs(speed));
		this->LEFT_MOTOR.rate.play(abs(speed));
	} else if (angle > 0) {
		// else if the angle is positive, speed down the right motor to turn right
		this->RIGHT_MOTOR.rate.play(abs(speed) * map(absAngle, 0, 90, 0, 100) / 100); // Good luck with this one...
		this->LEFT_MOTOR.rate.play(abs(speed));
	} else if (angle < 0) {
		// else if the angle is negative, speed down the left motor to turn left
		this->RIGHT_MOTOR.rate.play(abs(speed));
		this->LEFT_MOTOR.rate.play(abs(speed) * map(absAngle, 0, 90, 0, 100) / 100);
	}


	// Stop the motors when the speed is null
	if (speed == 0) {
		this->areEnabled = false;
		this->RIGHT_MOTOR.rate.stop();
		this->LEFT_MOTOR.rate.stop();
		digitalWrite(this->RIGHT_MOTOR.ENABLE_PIN, HIGH);
		digitalWrite(this->LEFT_MOTOR.ENABLE_PIN, HIGH);
	}
}
