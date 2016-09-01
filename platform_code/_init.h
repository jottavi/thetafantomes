#ifndef _init
#define _init

// ------------------------------------- Program parameters -------------------------------------


// This is where you change the player number
// It will affect the HOSTNAME and the IP which it's listening to
// DO NOT SWITCH THE WIFI MODULES (ESP8266), AND USE THE SAME PLAYER ID AS THE PLATEFORM NAME
// BECAUSE IT HAS THE RIGHT MAC ADDRESS REFERING TO THE RIGHT IP ADDRESS
#define PLAYER_ID 1


// WIFI

#define WIFI_SSID F("Theta")
#define WIFI_PASSWORD F("fant0m3s")
#define SERVER_PORT 7110

#if PLAYER_ID == 1
	#define PLAYER_IP "192.168.168.101"
	#define HOSTNAME "Alpha"
#elif PLAYER_ID == 2
	#define PLAYER_IP "192.168.168.102"
	#define HOSTNAME "Bravo"
#elif PLAYER_ID == 3
	#define PLAYER_IP "192.168.168.103"
	#define HOSTNAME "Charlie"
#elif PLAYER_ID == 4
	#define PLAYER_IP "192.168.168.104"
	#define HOSTNAME "Tango"
#endif


// CONSTANTS

#define MAX_DISTANCE 400
#define MAX_SPEED 10000

#define FORWARDS true
#define BACKWARDS false

#define SONAR1_ANGLE   -50
#define SONAR2_ANGLE   -25
#define SONAR3_ANGLE   0
#define SONAR4_ANGLE   25
#define SONAR5_ANGLE   50
#define SONAR6_ANGLE   130
#define SONAR7_ANGLE   155
#define SONAR8_ANGLE   180
#define SONAR9_ANGLE   -155
#define SONAR10_ANGLE  -130


// PINS

#define WIFI_RESET_PIN 6
#define WIFI_LED_PIN 13

#define LEFT_MOTOR_ENABLE_PIN      32
#define LEFT_MOTOR_DIRECTION_PIN   22
#define LEFT_MOTOR_STEP_PIN        24
#define LEFT_MOTOR_MS1_PIN         30
#define LEFT_MOTOR_MS2_PIN         28
#define LEFT_MOTOR_MS3_PIN         26

#define RIGHT_MOTOR_ENABLE_PIN     52
#define RIGHT_MOTOR_DIRECTION_PIN  42
#define RIGHT_MOTOR_STEP_PIN       44
#define RIGHT_MOTOR_MS1_PIN        50
#define RIGHT_MOTOR_MS2_PIN        48
#define RIGHT_MOTOR_MS3_PIN        46

#define SONAR1_TRIGGER_PIN   23
#define SONAR1_ECHO_PIN      25
#define SONAR2_TRIGGER_PIN   27
#define SONAR2_ECHO_PIN      29
#define SONAR3_TRIGGER_PIN   31
#define SONAR3_ECHO_PIN      33
#define SONAR4_TRIGGER_PIN   35
#define SONAR4_ECHO_PIN      37
#define SONAR5_TRIGGER_PIN   39
#define SONAR5_ECHO_PIN      41
#define SONAR6_TRIGGER_PIN   43
#define SONAR6_ECHO_PIN      45
#define SONAR7_TRIGGER_PIN   47
#define SONAR7_ECHO_PIN      49
#define SONAR8_TRIGGER_PIN   51
#define SONAR8_ECHO_PIN      53
#define SONAR9_TRIGGER_PIN   40
#define SONAR9_ECHO_PIN      38
#define SONAR10_TRIGGER_PIN  36
#define SONAR10_ECHO_PIN     34

#endif
