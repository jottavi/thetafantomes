#ifndef motors
#define motors

#include <Arduino.h>
#include <Tone.h>

#include "_init.h"
#include "motion.h"

typedef struct Motor {
	const int ENABLE_PIN;
	const int DIRECTION_PIN;
	const int STEP_PIN;
	const int MS1_PIN;
	const int MS2_PIN;
	const int MS3_PIN;
	Tone rate;
};


class Motors_C {
	public:
	Motors_C();
	void init();
	void move(Motion_C& motion_);

	private:
	void updateDirection(Motion_C motion_);
	void updateSpeed(Motion_C motion_);
	Motor RIGHT_MOTOR;
	Motor LEFT_MOTOR;
	bool areEnabled;
};

#endif
