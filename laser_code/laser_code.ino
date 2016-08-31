//int pin1 = 0; GPIO0
//int pin2 = 1; BUILT IN LED
//int pin3 = 2; GPIO2

// ------------------------------------- INCLUDES -------------------------------------

#include <DmxSimple.h>
#include <OSCMessage.h>

#include "_init.h"
#include "wifi.h"
#include "osc.h"
#include "laser.h"
#include "motion.h"


// ------------------------------------- VARIABLES -------------------------------------


// Communication
Osc_C osc_;
Wifi_C wifi_;


// Lasers
Laser_C laser_[4] = {
	Laser_C(0, PLAYER1_IP),
	Laser_C(1, PLAYER2_IP),
	Laser_C(2, PLAYER3_IP),
	Laser_C(3, PLAYER4_IP)
};
Motion_C motion_[4];


// ------------------------------------- SETUP -------------------------------------

void setup() {
	Serial.begin(115200); // Serial debug via USB

	Serial.println("****************************************************************");
	Serial.println("************************ Starting setup ************************");
	Serial.println("****************************************************************\n");

	for(int i = 0; i < 4; i++) {
		laser_[i].init();
	}

	wifi_.init();

	Serial.println("****************************************************************");
	Serial.println("*************************** Setup OK ***************************"); // Everything's up and running
	Serial.println("****************************************************************\n");
}


// ------------------------------------- LOOP --------------------------------------

void loop() {
	wifi_.receive(osc_); // Grab and parse the OSC message from the Wifi chip (osc.cpp)

	for (int i = 0; i < 4; i++) {
		if (osc_.getIp() == laser_[i].getIp()) {
			motion_[i].update(osc_);
			laser_[i].move(motion_[i]);
		}
	}

	osc_.clean();
}
