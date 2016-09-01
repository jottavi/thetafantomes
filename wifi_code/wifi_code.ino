// Copyright (C) 2012, 2013 APO-33
// This file is part of the Thêta Fantômes project.
//
// Thêta Fantômes is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Thêta Fantômes is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <esp8266-OSCMessage.h>

// This is necessary in order to change the HOSTNAME
extern "C" {
	#include "user_interface.h"
}

// The code is supporting softwareSerial, which is slower but allows more debuging verbose (than the hardware serial)
#define USE_SOFTWARE_SERIAL false

#if USE_SOFTWARE_SERIAL == true
	#define BAUDRATE 9600
#elif USE_SOFTWARE_SERIAL == false
	#define BAUDRATE 115200
#endif

// Documentation of the OSC library can be found here : https://github.com/CNMAT/OSC
// And the rest of the API is available here : http://cnmat.berkeley.edu/library/oscuino/
// Here is the same library adapted for the esp8266 : https://github.com/sandeepmistry/esp8266-OSC

String SSID;
String PASSWORD;
String HOSTNAME;
int SERVER_PORT;
byte mac[6];

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

OSCErrorCode error;


void setup() {
	// Serial connection to the arduino
	Serial.begin(BAUDRATE);

	// Give time to the arduino
	delay(200);

	// Let's go
	Serial.println();
	Serial.println("Wifi chip ready");
	Serial.println();

	// Waiting for the SSID
	Serial.println("SSID?");
	SSID = getSerialMessage();

	// Waiting for the PASSWORD
	Serial.println("PASSWORD?");
	PASSWORD = getSerialMessage();

	// Waiting for the SERVER_PORT
	Serial.println("SERVER_PORT?");
	SERVER_PORT = getSerialMessage().toInt();

	// Waiting for the HOSTNAME
	Serial.println("HOSTNAME?");
	HOSTNAME = getSerialMessage();

	// Converting everything from Strings to char arrays
	char ssid[SSID.length() + 1];
	SSID.toCharArray(ssid, SSID.length() + 1);

	char pass[PASSWORD.length() + 1];
	PASSWORD.toCharArray(pass, PASSWORD.length() + 1);

	char hostname[HOSTNAME.length() + 1];
	HOSTNAME.toCharArray(hostname, HOSTNAME.length() + 1);

	// Set up the hostname and the wifi mode
	wifi_station_set_hostname(hostname);
	WiFi.mode(WIFI_STA);

	// Some verbose
	Serial.println();
	Serial.print("*** Connecting to ");
	Serial.print(ssid);
	Serial.println(" ***");
	Serial.println();
	Serial.println();

	// Connecting to wifi network
	WiFi.begin(ssid, pass);
	Serial.println("WiFi connected");

	// Print out the MAC address to the arduino
	WiFi.macAddress(mac);
	Serial.print("MAC : ");
	Serial.print(mac[0],HEX);
	Serial.print(":");
	Serial.print(mac[1],HEX);
	Serial.print(":");
	Serial.print(mac[2],HEX);
	Serial.print(":");
	Serial.print(mac[3],HEX);
	Serial.print(":");
	Serial.print(mac[4],HEX);
	Serial.print(":");
	Serial.println(mac[5],HEX);

	// Print out the IP address to the arduino (does not work for now)
	Serial.print("IP address : ");
	Serial.println(WiFi.localIP());

	// Start the UDP server and print out the port to the arduino
	Serial.println();
	Serial.println("Starting UDP");
	Udp.begin(SERVER_PORT);
	Serial.print("Local port : ");
	Serial.println(Udp.localPort());
	Serial.println();
	Serial.println("All OK");
	Serial.println();
}


String getSerialMessage() {
	// Wait for the next serial message and return it
	String cache;
	while(1){
		if(Serial.available()) {
			cache = Serial.readStringUntil('\n');
			cache.trim();
			break;
		}
	}
	return cache;
}


void loop() {
	OSCMessage message;
	int value = 0;
	char route[200];

	// Get the size of the incoming packet and its remote IP
	int size = Udp.parsePacket();
	IPAddress remoteIp = Udp.remoteIP(); // IP of the sender

	// If there is a packet
	if (size > 0) {
		// Get every byte of the message from this packet
		while (size--) {
			message.fill(Udp.read());
		}

		if (!message.hasError()) {
			// If the message is sane, unpack it and send it to the arduino
			message.getAddress(route);
			value = message.getInt(0);

			Serial.print("OSC ");
			Serial.print(remoteIp);
			Serial.print(" ");
			Serial.print(route);
			Serial.print(" ");
			Serial.println(value);
		} else {
			// If the message is not sane, print out the error
			error = message.getError();
			Serial.print("error: ");
			Serial.println(error);
		}
	}
}
