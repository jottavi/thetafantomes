#include "sonars.h"

Sonars_C::Sonars_C() :
	sonars_ {
		{
			NewPing(SONAR1_TRIGGER_PIN, SONAR1_ECHO_PIN, MAX_DISTANCE),
			SONAR1_ANGLE,
			MAX_DISTANCE,
			SONAR1_TRIGGER_PIN,
			SONAR1_ECHO_PIN
		}, {
			NewPing(SONAR2_TRIGGER_PIN, SONAR2_ECHO_PIN, MAX_DISTANCE),
			SONAR2_ANGLE,
			MAX_DISTANCE,
			SONAR2_TRIGGER_PIN,
			SONAR2_ECHO_PIN
		}, {
			NewPing(SONAR3_TRIGGER_PIN, SONAR3_ECHO_PIN, MAX_DISTANCE),
			SONAR3_ANGLE,
			MAX_DISTANCE,
			SONAR3_TRIGGER_PIN,
			SONAR3_ECHO_PIN
		}, {
			NewPing(SONAR4_TRIGGER_PIN, SONAR4_ECHO_PIN, MAX_DISTANCE),
			SONAR4_ANGLE,
			MAX_DISTANCE,
			SONAR4_TRIGGER_PIN,
			SONAR4_ECHO_PIN
		}, {
			NewPing(SONAR5_TRIGGER_PIN, SONAR5_ECHO_PIN, MAX_DISTANCE),
			SONAR5_ANGLE,
			MAX_DISTANCE,
			SONAR5_TRIGGER_PIN,
			SONAR5_ECHO_PIN
		}, {
			NewPing(SONAR6_TRIGGER_PIN, SONAR6_ECHO_PIN, MAX_DISTANCE),
			SONAR6_ANGLE,
			MAX_DISTANCE,
			SONAR6_TRIGGER_PIN,
			SONAR6_ECHO_PIN
		}, {
			NewPing(SONAR7_TRIGGER_PIN, SONAR7_ECHO_PIN, MAX_DISTANCE),
			SONAR7_ANGLE,
			MAX_DISTANCE,
			SONAR7_TRIGGER_PIN,
			SONAR7_ECHO_PIN
		}, {
			NewPing(SONAR8_TRIGGER_PIN, SONAR8_ECHO_PIN, MAX_DISTANCE),
			SONAR8_ANGLE,
			MAX_DISTANCE,
			SONAR8_TRIGGER_PIN,
			SONAR8_ECHO_PIN
		}, {
			NewPing(SONAR9_TRIGGER_PIN, SONAR9_ECHO_PIN, MAX_DISTANCE),
			SONAR9_ANGLE,
			MAX_DISTANCE,
			SONAR9_TRIGGER_PIN,
			SONAR9_ECHO_PIN
		}, {
			NewPing(SONAR10_TRIGGER_PIN, SONAR10_ECHO_PIN, MAX_DISTANCE),
			SONAR10_ANGLE,
			MAX_DISTANCE,
			SONAR10_TRIGGER_PIN,
			SONAR10_ECHO_PIN
		},
	},
	distanceThreshold(200),
	isDetectingObjects(false),
	objectAngle(0),
	sensorsCheckTimeout(0)
{}


void Sonars_C::init() {
	for(int i = 0; i < 10; i++){
		this->reset(i);
	}

	delay(300);

	this->read();

	Serial.println("Checking Sonars' hardware condition...");

	for(int i = 0; i < 10; i++) {
		if (this->sonars_[i].value <= 0) {
			Serial.print("Sonar ");
			Serial.print(i);
			Serial.println(" : Failure");
		} else if (this->sonars_[i].value > 0) {
			Serial.print("Sonar ");
			Serial.print(i);
			Serial.println(" : OK");
		}
	}

	Serial.println();
}


void Sonars_C::correct(Motion_C& motion_) {
	if(this->sensorsCheckTimeout + 300 < millis()) {
		this->read();
		this->sensorsCheckTimeout = millis();
	}

	this->defineDistanceThreshold(motion_);
	this->detectObjects();

	if (this->isDetectingObjects == true) {
		this->chooseDirection(motion_);
	}
}


void Sonars_C::read() {
	String message = "";

	for(int i = 0; i < 10; i++) {
		int sonarValue = this->sonars_[i].sensor.ping() / US_ROUNDTRIP_CM;

		if(sonarValue != 400 || sonarValue != 0) {
			this->sonars_[i].value = sonarValue;
		} else {
			this->reset(i);
		}

		message = message + "Sonar ";
		message = message + i;
		message = message + " : ";
		message = message + this->sonars_[i].value;
		message = message + " cm\n";
	}

	Serial.println(message);
}


void Sonars_C::reset(int i) {
	pinMode(this->sonars_[i].ECHO_PIN, OUTPUT);
	digitalWrite(this->sonars_[i].ECHO_PIN, LOW);
	pinMode(this->sonars_[i].ECHO_PIN, INPUT);
}


void Sonars_C::defineDistanceThreshold(Motion_C& motion_) {
	int speed = abs(motion_.getSpeed());

	if (speed <= 0.3 * MAX_SPEED) {
		this->distanceThreshold = 50;
	} else if (speed > 0.3 * MAX_SPEED) {
		int speedRange = MAX_SPEED - 0.3 * MAX_SPEED;
		int setSpeed = speed - 0.3 * MAX_SPEED;
		this->distanceThreshold = setSpeed * 150 / speedRange + 50;
	}
}


void Sonars_C::detectObjects() {
	int shortestDistance = 200;

	for(int i = 0; i < 10; i++) {
		if (this->sonars_[i].value < distanceThreshold && this->sonars_[i].value < shortestDistance) {
			shortestDistance = this->sonars_[i].value;
			this->isDetectingObjects = true;
			this->objectAngle = this->sonars_[i].ANGLE;
		}
	}

	if (shortestDistance == 200) {
		this->isDetectingObjects = false;
		this->objectAngle = 0;
	}
}


void Sonars_C::chooseDirection(Motion_C& motion_) {
	// Direction & objects states
	// 1 : First quarter
	// 2 : Straight forwards
	// 3 : Second quarter
	// 4 : Third quarter
	// 5 : Straight backwards
	// 6 : Fourth quarter

	int directionState = 0;
	int objectState = 0;

	// Determining the plateform direction
	if (motion_.getAngle() < 0 && motion_.getSpeed() > 0) {
		directionState = 1;
	} else if (motion_.getAngle() == 0 && motion_.getSpeed() > 0) {
		directionState = 2;
	} else if (motion_.getAngle() > 0 && motion_.getSpeed() > 0) {
		directionState = 3;
	} else if (motion_.getAngle() < 0 && motion_.getSpeed() < 0) {
		directionState = 4;
	} else if (motion_.getAngle() == 0 && motion_.getSpeed() < 0) {
		directionState = 5;
	} else if (motion_.getAngle() > 0 && motion_.getSpeed() < 0) {
		directionState = 6;
	}


	// Determining the object position
	if (-150 < motion_.getAngle() && motion_.getAngle() < 150) {
		if (-90 < this->objectAngle < 0 && motion_.getSpeed() > 0) {
			objectState = 1;
		} else if (this->objectAngle == 0 && motion_.getSpeed() > 0) {
			objectState = 2;
		} else if (0 < this->objectAngle < 90 && motion_.getSpeed() > 0) {
			objectState = 3;
		} else if (90 < this->objectAngle < 180 && motion_.getSpeed() < 0) {
			objectState = 4;
		} else if (this->objectAngle == 180 && motion_.getSpeed() < 0) {
			objectState = 5;
		} else if (-180 < this->objectAngle < -90 && motion_.getSpeed() < 0) {
			objectState = 6;
		}
	}


	if (directionState == objectState) {
		motion_.switchPolarity();
	}
}
