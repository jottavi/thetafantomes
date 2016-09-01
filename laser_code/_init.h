#ifndef _init
#define _init

// ------------------------------------- Program parameters -------------------------------------


// when sofwareSerial is used, the connection with the wifi module is on pin 5 (RX) and 6 (TX)
// so if you use it, make sure the jumpers are switched on the shield
#define USE_SOFTWARE_SERIAL false


// --------- WIFI

#define WIFI_SSID F("Theta")
#define WIFI_PASSWORD F("fant0m3s")
#define SERVER_PORT 7110
#define HOSTNAME "Lasers"

// IPs the lasers are LISTENING to
#define PLAYER1_IP "192.168.168.101"
#define PLAYER2_IP "192.168.168.102"
#define PLAYER3_IP "192.168.168.103"
#define PLAYER4_IP "192.168.168.104"
#define MASTER_IP "192.168.168.100"


// --------- DMX

#define DMX_DEVICE_CHANNEL_NB 8
// This is the number of channels which are allowed for each DMX device
// The first one will be adressed in channel 1, the second in channel 9, the third in channel 17 and so on...


// --------- PINS

#define WIFI_RESET_PIN 7
#define WIFI_LED_PIN 13
#define WIFI_SOFTWARE_RX_PIN 5
#define WIFI_SOFTWARE_TX_PIN 6


#endif
