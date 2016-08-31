#include "wifi.h"

Wifi_C::Wifi_C(){}


void Wifi_C::init() {
	String cache = "";
	bool chipIsReady = false;

	pinMode(WIFI_LED_PIN, OUTPUT);
	pinMode(WIFI_RESET_PIN, OUTPUT);

	digitalWrite(WIFI_LED_PIN, LOW);
	digitalWrite(WIFI_RESET_PIN, HIGH);
	delay(5);
	digitalWrite(WIFI_RESET_PIN, LOW);
	delay(5);

	Serial3.begin(115200); // Serial connection to ESP8266

	while(1){
		if(Serial3.available()) {
			cache = Serial3.readStringUntil('\n');

			if(cache.indexOf("Wifi chip ready") != -1) {
				chipIsReady = true;
			}
			if(chipIsReady) {
				Serial.println(cache);
			}

			if(cache.indexOf("SSID?") != -1) {
				Serial3.println(WIFI_SSID);
				Serial.println(WIFI_SSID);
				Serial.println();
			} else if(cache.indexOf("PASSWORD?") != -1) {
				Serial3.println(WIFI_PASSWORD);
				Serial.println(WIFI_PASSWORD);
				Serial.println();
			} else if(cache.indexOf("SERVER_PORT?") != -1) {
				Serial3.println(SERVER_PORT);
				Serial.println(SERVER_PORT);
				Serial.println();
			} else if(cache.indexOf("HOSTNAME?") != -1) {
				Serial3.println(HOSTNAME);
				Serial.println(HOSTNAME);
				Serial.println();
			} else if (cache.indexOf("All OK") != -1) {
				Serial.println();
				digitalWrite(WIFI_LED_PIN, HIGH);
				break;
			}
		}
	}
}


void Wifi_C::receive(Osc_C& osc_) {
	// receive OSC from the wifi module
	String cache = Serial3.readStringUntil('\n');

	if (cache.indexOf("OSC") != -1) {
		// parse it, build it
		osc_.unpack(cache);
	}
}