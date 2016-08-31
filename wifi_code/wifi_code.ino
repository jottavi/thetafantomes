#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <esp8266-OSCMessage.h>

extern "C" {
	#include "user_interface.h"
}

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

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
byte mac[6];
const unsigned int localPort = 8888; // local port to listen for UDP packets (here's where we send the packets)

OSCErrorCode error;


void setup() {
	Serial.begin(BAUDRATE);

	delay(200);

	Serial.println();
	Serial.println("Wifi chip ready");
	Serial.println();

	Serial.println("SSID?");
	SSID = getSerialMessage();

	Serial.println("PASSWORD?");
	PASSWORD = getSerialMessage();

	Serial.println("SERVER_PORT?");
	SERVER_PORT = getSerialMessage().toInt();

	Serial.println("HOSTNAME?");
	HOSTNAME = getSerialMessage();

	char ssid[SSID.length() + 1];
	SSID.toCharArray(ssid, SSID.length() + 1);

	char pass[PASSWORD.length() + 1];
	PASSWORD.toCharArray(pass, PASSWORD.length() + 1);

	char hostname[HOSTNAME.length() + 1];
	HOSTNAME.toCharArray(hostname, HOSTNAME.length() + 1);

	// Connect to WiFi network
	wifi_station_set_hostname(hostname);
	WiFi.mode(WIFI_STA);

	Serial.println();
	Serial.print("*** Connecting to ");
	Serial.print(ssid);
	Serial.println(" ***");
	Serial.println();
	Serial.println();

	WiFi.begin(ssid, pass);

	Serial.println("WiFi connected");

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

	Serial.print("IP address : ");
	Serial.println(WiFi.localIP());

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
	int size = Udp.parsePacket();
	int value = 0;
	char route[200];
	IPAddress remoteIp = Udp.remoteIP(); // => IP of the sender

	if (size > 0) {
		while (size--) {
			message.fill(Udp.read());
		}

		if (!message.hasError()) {
			message.getAddress(route);
			value = message.getInt(0);

			Serial.print("OSC");
			Serial.print(" ");
			Serial.print(remoteIp);
			Serial.print(" ");
			Serial.print(route);
			Serial.print(" ");
			Serial.println(value);
		} else {
			error = message.getError();
			Serial.print("error: ");
			Serial.println(error);
		}
	}
}