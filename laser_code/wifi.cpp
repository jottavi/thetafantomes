#include "wifi.h"

Wifi_C::Wifi_C() :
	wifiSerial {
		WIFI_RX_PIN,
		WIFI_TX_PIN
	}
{}


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

	if (USE_SOFTWARE_SERIAL) {
		wifiSerial.begin(9600); // Serial connection to ESP8266

		while(1){
			if(wifiSerial.available()) {
				cache = wifiSerial.readStringUntil('\n');

				if(cache.indexOf("Wifi chip ready") != -1) {
					chipIsReady = true;
				}
				if(chipIsReady) {
					Serial.println(cache);
				}

				if(cache.indexOf("SSID?") != -1) {
					wifiSerial.println(WIFI_SSID);
					Serial.println(WIFI_SSID);
					Serial.println();
				} else if(cache.indexOf("PASSWORD?") != -1) {
					wifiSerial.println(WIFI_PASSWORD);
					Serial.println(WIFI_PASSWORD);
					Serial.println();
				} else if(cache.indexOf("SERVER_PORT?") != -1) {
					wifiSerial.println(SERVER_PORT);
					Serial.println(SERVER_PORT);
					Serial.println();
				} else if(cache.indexOf("HOSTNAME?") != -1) {
					wifiSerial.println(HOSTNAME);
					Serial.println(HOSTNAME);
					Serial.println();
				} else if (cache.indexOf("All OK") != -1) {
					Serial.println();
					digitalWrite(WIFI_LED_PIN, HIGH);
					break;
				}
			}
		}
	} else {
		Serial.begin(115200);

		while(1){
			if(Serial.available()) {
				cache = Serial.readStringUntil('\n');

				if(cache.indexOf("Wifi chip ready") != -1) {
					chipIsReady = true;
				}

				if(cache.indexOf("SSID?") != -1) {
					Serial.println(WIFI_SSID);
				} else if(cache.indexOf("PASSWORD?") != -1) {
					Serial.println(WIFI_PASSWORD);
				} else if(cache.indexOf("SERVER_PORT?") != -1) {
					Serial.println(SERVER_PORT);
				} else if(cache.indexOf("HOSTNAME?") != -1) {
					Serial.println(HOSTNAME);
				} else if (cache.indexOf("All OK") != -1) {
					digitalWrite(WIFI_LED_PIN, HIGH);
					break;
				}
			}
		}
	}
}


void Wifi_C::receive(Osc_C& osc_) {
	// receive OSC from the wifi module
	String cache = "";

	if (USE_SOFTWARE_SERIAL) {
		cache = wifiSerial.readStringUntil('\n');
	} else {
		cache = Serial.readStringUntil('\n');
	}

	if (cache.indexOf("OSC") != -1) {
		// parse it, build it
		osc_.unpack(cache);
	}
}