// Copyright (C) 2012, 2013 APO-33
// Author : Matisse V (http://github.com/matissev)
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
//
//
//				                  `/shmNMMMMNmho:`                 
//				                :hMms/-.`  `.-+yNNy.               
//				              `hMh-              /mMo              
//				             `mMo                 `yMy             
//				             dMo                    hMs            
//				            oMh   `oo`    `oo`      `mM/           
//				           -MN.   mMMm    mMMm       -MN.          
//				          -NM:    mMMm    mMMm        +Md.         
//				      .+ymMd-     `++`    `++`         /mMmho:`    
//				    /dMdo:`                              `-+yNNy.  
//				  .dMh-                                       /NM/ 
//				 `mM/                                          .NM:
//				 yMs                                            +Mh
//				 MM+-`                                          .MN
//				 shdmMmy-                                     ..-MM
//				     `/dMs                                /hNMNmmmd
//				       `mM:                              yMh:`     
//				        sMs                             /Md        
//				        /Md                            `mM:        
//				         hMds/-                       :dMo         
//				          -+shNMd+`               :shNNy-          
//				               .sMm-            .dMh+-             
//				                 -NN:          /NN:                
//				                  .dMh/-`  `-+dMd.                 
//				                    -sdNMMMMmho-      
//


// ------------------------------------- INCLUDES -------------------------------------

#include <Tone.h>
#include <NewPing.h>
// Here is a stackoverflow that will help you to resolve the timer compilation error (vector_13), if needed.
// http://stackoverflow.com/questions/17208703/arduino-timer-seems-to-be-crashing
// In NewPing.cpp, also consider replacing :
// TIMER2_COMPA_vect (l.284)
// with
// TIMER3_COMPA_vect

#include "_init.h" // This file contain the constants, like the SSID, the pinout, etc..
#include "wifi.h"
#include "osc.h"
#include "motors.h"
#include "motion.h"
#include "sonars.h"


// ------------------------------------- CLASS -------------------------------------


// Communication
Osc_C osc_;
Wifi_C wifi_;


// Motors
Motors_C motors_;
Motion_C motion_;


// Sensors
Sonars_C sonars_;


// ------------------------------------- SETUP -------------------------------------

void setup() {
	Serial.begin(115200); // Serial debug via USB

	Serial.println("****************************************************************");
	Serial.println("************************ Starting setup ************************");
	Serial.println("****************************************************************\n");

	motors_.init(); // Setting up the motors pins
	wifi_.init(); // Connecting to the wifi network
	sonars_.init(); // Checking the sonars condition

	Serial.println("****************************************************************");
	Serial.println("*************************** Setup OK ***************************"); // Everything's up and running
	Serial.println("****************************************************************\n");
}


// ------------------------------------- LOOP --------------------------------------

void loop() {
	// Update the motion values with the incoming message, line it
	// motion.cpp - osc.cpp
	motion_.update(osc_);

	// Invert direction when an object is in the way
	// sonars.cpp - motion.cpp
	// sonars_.correct(motion_);

	// Move according to the motion calculated and save it
	// motors.cpp - motion.cpp
	motors_.move(motion_);
}

void serialEvent3() {
	// Grab and parse the OSC message from the Serial
	// wifi.cpp - osc.cpp
	wifi_.receive(osc_);
}
