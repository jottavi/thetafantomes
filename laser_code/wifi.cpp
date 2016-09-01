#include "wifi.h"

Wifi_C::Wifi_C() :
	wifiSerial {
		WIFI_SOFTWARE_RX_PIN,
		WIFI_SOFTWARE_TX_PIN
	}
{}


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


	if (USE_SOFTWARE_SERIAL) {
		// Code supporting softwareSerial, which is slower but allows more debuging verbose (than the hardware serial)

		// Serial connection to the wifi module (ESP8266)
		wifiSerial.begin(9600);

		// Serial debug via USB
		Serial.begin(115200);

		while(1){
			// Wifi module, let's talk man.
			if(wifiSerial.available()) {
				// Store the message from the wifi module.
				cache = wifiSerial.readStringUntil('\n');

				// Waiting for the module to say that it's ready
				if(cache.indexOf("Wifi chip ready") != -1) {
					chipIsReady = true;
				}

				// Print the debug when it is relevant
				if(chipIsReady) {
					Serial.println(cache);
				}

				if(cache.indexOf("SSID?") != -1) {
					// Wifi module is asking the SSID, give it to him
					wifiSerial.println(WIFI_SSID);
					Serial.println(WIFI_SSID);
					Serial.println();
				} else if(cache.indexOf("PASSWORD?") != -1) {
					// Wifi module is asking the PASSWORD, give it to him
					wifiSerial.println(WIFI_PASSWORD);
					Serial.println(WIFI_PASSWORD);
					Serial.println();
				} else if(cache.indexOf("SERVER_PORT?") != -1) {
					// Wifi module is asking the SERVER_PORT, give it to him
					wifiSerial.println(SERVER_PORT);
					Serial.println(SERVER_PORT);
					Serial.println();
				} else if(cache.indexOf("HOSTNAME?") != -1) {
					// Wifi module is asking the HOSTNAME, give it to him
					wifiSerial.println(HOSTNAME);
					Serial.println(HOSTNAME);
					Serial.println();
				} else if (cache.indexOf("All OK") != -1) {
					// Let's rock
					Serial.println();
					digitalWrite(WIFI_LED_PIN, HIGH); // LED is on, and telling that everything went through
					break;
				}
			}
		}
	} else {
		// Code supporting the hardware serial communication, which is faster but allows less debuging verbose (than softwareSerial)

		// Serial connection to the wifi module (ESP8266)
		Serial.begin(115200);

		while(1){
			// Wifi module, let's talk man.
			if(Serial.available()) {
				// Store the message from the wifi module.
				cache = Serial.readStringUntil('\n');

				// Waiting for the module to say that it's ready
				if(cache.indexOf("Wifi chip ready") != -1) {
					chipIsReady = true;
				}

				if(cache.indexOf("SSID?") != -1) {
					// Wifi module is asking the SSID, give it to him
					Serial.println(WIFI_SSID);
				} else if(cache.indexOf("PASSWORD?") != -1) {
					// Wifi module is asking the PASSWORD, give it to him
					Serial.println(WIFI_PASSWORD);
				} else if(cache.indexOf("SERVER_PORT?") != -1) {
					// Wifi module is asking the SERVER_PORT, give it to him
					Serial.println(SERVER_PORT);
				} else if(cache.indexOf("HOSTNAME?") != -1) {
					// Wifi module is asking the HOSTNAME, give it to him
					Serial.println(HOSTNAME);
				} else if (cache.indexOf("All OK") != -1) {
					// Let's rock
					digitalWrite(WIFI_LED_PIN, HIGH); // LED is on, and telling that everything went through
					break;
				}
			}
		}
	}
}


void Wifi_C::receive(Osc_C& osc_) {
	String cache = "";

	// read the message from the wifi module
	if (USE_SOFTWARE_SERIAL) {
		cache = wifiSerial.readStringUntil('\n');
	} else {
		cache = Serial.readStringUntil('\n');
	}

	// if it is indeed an OSC message, parse it, build it
	if (cache.indexOf("OSC") != -1) {
		osc_.unpack(cache);
	}
}