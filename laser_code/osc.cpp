#include "osc.h"

Osc_C::Osc_C() :
	ip(""),
	route(""),
	value(0)
{}


void Osc_C::unpack(String cache) {
	// find the route and the value of the OSC message in cache and cut it
	int ipIndex = cache.indexOf(' ') + 1;
	int routeIndex = cache.indexOf('/');
	int valueIndex = cache.lastIndexOf(' ') + 1;
	this->ip = cache.substring(ipIndex, routeIndex - 1);
	this->ip.trim();
	this->route = cache.substring(routeIndex, valueIndex);
	this->route.trim();
	this->value = cache.substring(valueIndex).toInt();

	// some little debug never hurt
	Serial.print("OSC ");
	Serial.print(this->ip);
	Serial.print(" ");
	Serial.print(this->route);
	Serial.print(" ");
	Serial.println(this->value);
}


void Osc_C::clean() {
	this->ip = "";
	this->route = "";
	this->value = 0;
}


String Osc_C::getIp() const {
	return this->ip;
}


String Osc_C::getRoute() const {
	return this->route;
}


int Osc_C::getValue() const {
	return this->value;
}