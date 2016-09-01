// ------------------------------------- INCLUDES -------------------------------------

#include <Tone.h>
#include <NewPing.h>
// Here is a stackoverflow that will help you to resolve the timer compilation error (vector_13), if needed.
// http://stackoverflow.com/questions/17208703/arduino-timer-seems-to-be-crashing
// In NewPing.cpp, also consider replacing :
// TIMER2_COMPA_vect (l.284)
// with
// TIMER3_COMPA_vect

#include "_init.h" // This file contain the constants, like the SSID, the pinout, etc..
#include "wifi.h"
#include "osc.h"
#include "motors.h"
#include "motion.h"
#include "sonars.h"


// ------------------------------------- CLASS -------------------------------------


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

	motors_.init(); // Setting up the motors pins
	wifi_.init(); // Connecting to the wifi network
	sonars_.init(); // Checking the sonars condition

	Serial.println("****************************************************************");
	Serial.println("*************************** Setup OK ***************************"); // Everything's up and running
	Serial.println("****************************************************************\n");
}


// ------------------------------------- LOOP --------------------------------------

void loop() {
	// Update the motion values with the incoming message, line it
	motion_.update(osc_); // motion.cpp - osc.cpp

	// Invert direction when an object is in the way
	sonars_.correct(motion_); // sonars.cpp - motion.cpp

	// Move according to the motion calculated and save it
	motors_.move(motion_); // motors.cpp - motion.cpp
}

void serialEvent3() {
	// Grab and parse the OSC message from the Serial
	wifi_.receive(osc_); // wifi.cpp - osc.cpp
}
