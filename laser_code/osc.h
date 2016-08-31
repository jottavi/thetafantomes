#ifndef osc
#define osc

#include <Arduino.h>
#include "_init.h"

class Osc_C {
	public:
	Osc_C();
	void clean();
	void unpack(String cache);
	String getIp() const;
	String getRoute() const;
	int getValue() const;

	private:
	String ip;
	String route;
	int value;
};

#endif
