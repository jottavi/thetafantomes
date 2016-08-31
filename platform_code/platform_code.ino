// ------------------------------------- INCLUDES -------------------------------------

#include <Tone.h>
#include <NewPing.h>
// Here is a stackoverflow that will help you to resolve the timer compilation error (vector_13), if needed.
// http://stackoverflow.com/questions/17208703/arduino-timer-seems-to-be-crashing
// In NewPing.cpp, also consider replacing :
// TIMER2_COMPA_vect (l.284)
// with
// TIMER3_COMPA_vect

#include "_init.h"
#include "wifi.h"
#include "osc.h"
#include "motors.h"
#include "motion.h"
#include "sonars.h"


// ------------------------------------- VARIABLES -------------------------------------


// Communication
Osc_C osc_;
Wifi_C wifi_;


// Motors
Motors_C motors_;
Motion_C motion_;


// Sensors
Sonars_C sonars_;

// ------------------------------------- SETUP -------------------------------------

void setup() {
	Serial.begin(115200); // Serial debug via USB

	Serial.println("****************************************************************");
	Serial.println("************************ Starting setup ************************");
	Serial.println("****************************************************************\n");

	motors_.init();
	wifi_.init();
	sonars_.init();

	Serial.println("****************************************************************");
	Serial.println("*************************** Setup OK ***************************"); // Everything's up and running
	Serial.println("****************************************************************\n");
}


// ------------------------------------- LOOP --------------------------------------

void loop() {
	if (osc_.getIp() == PLAYER_IP) {
		motion_.update(osc_);
	} else {
		osc_.clean();
	}

	sonars_.correct(motion_);

	if (motion_.hasChanged()) {
		motors_.move(motion_);
		motion_.saveValues();
	}
}

void serialEvent3() {
	wifi_.receive(osc_); // Grab and parse the OSC message from the Serial (osc.cpp)
}
