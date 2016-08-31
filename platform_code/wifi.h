#ifndef wifi
#define wifi

#include <Arduino.h>

#include "_init.h"
#include "osc.h"

class Wifi_C {
	public:
	Wifi_C();
	void init();
	void receive(Osc_C& osc_);
};

#endif