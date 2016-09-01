#include "wifi.h"

Wifi_C::Wifi_C(){}


void Wifi_C::init() {
	String cache = "";
	bool chipIsReady = false;

	// Defining the pins
	pinMode(WIFI_LED_PIN, OUTPUT);
	pinMode(WIFI_RESET_PIN, OUTPUT);

	// Reset of the wifi module and switching off the led
	digitalWrite(WIFI_LED_PIN, LOW);
	digitalWrite(WIFI_RESET_PIN, HIGH);
	delay(5);
	digitalWrite(WIFI_RESET_PIN, LOW);
	delay(5);

	Serial3.begin(115200); // Serial connection to the wifi module (ESP8266)

	while(1){
		if(Serial3.available()) { // Wifi module, let's talk man.
			cache = Serial3.readStringUntil('\n');

			if(cache.indexOf("Wifi chip ready") != -1) { // Waiting for the module to say that it's ready
				chipIsReady = true;
			}

			if(chipIsReady) { // Print the debug when it is relevant
				Serial.println(cache);
			}

			if(cache.indexOf("SSID?") != -1) { // Wifi module is asking the SSID, tell it to him
				Serial3.println(WIFI_SSID);
				Serial.println(WIFI_SSID);
				Serial.println();
			} else if(cache.indexOf("PASSWORD?") != -1) { // Wifi module is asking the PASSWORD, tell it to him
				Serial3.println(WIFI_PASSWORD);
				Serial.println(WIFI_PASSWORD);
				Serial.println();
			} else if(cache.indexOf("SERVER_PORT?") != -1) { // Wifi module is asking the SERVER_PORT, tell it to him
				Serial3.println(SERVER_PORT);
				Serial.println(SERVER_PORT);
				Serial.println();
			} else if(cache.indexOf("HOSTNAME?") != -1) { // Wifi module is asking the SERVER_PORT, tell it to him
				Serial3.println(HOSTNAME);
				Serial.println(HOSTNAME);
				Serial.println();
			} else if (cache.indexOf("All OK") != -1) { // Let's rock
				Serial.println();
				digitalWrite(WIFI_LED_PIN, HIGH); // LED is on
				break;
			}
		}
	}
}


void Wifi_C::receive(Osc_C& osc_) {
	// read the message from the wifi module
	String cache = Serial3.readStringUntil('\n');

	// if it is indeed an OSC message, parse it, build it
	if (cache.indexOf("OSC") != -1) {
		osc_.unpack(cache);
	}
}