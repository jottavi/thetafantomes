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

#include <DmxSimple.h>
#include <OSCMessage.h>

#include "_init.h" // This file contain the constants, like the SSID, the pinout, etc..
#include "wifi.h"
#include "osc.h"
#include "laser.h"
#include "motion.h"


// ------------------------------------- CLASS -------------------------------------


// Communication
Osc_C osc_;
Wifi_C wifi_;


// Lasers
Laser_C laser_[4] = {
	Laser_C(0, PLAYER1_IP),
	Laser_C(1, PLAYER2_IP),
	Laser_C(2, PLAYER3_IP),
	Laser_C(3, PLAYER4_IP)
};
Motion_C motion_[4];


// ------------------------------------- SETUP -------------------------------------

void setup() {
	Serial.println("****************************************************************");
	Serial.println("************************ Starting setup ************************");
	Serial.println("****************************************************************\n");

	// Setting up the lasers pins
	for(int i = 0; i < 4; i++) {
		laser_[i].init();
	}

	// Connecting to the wifi network
	wifi_.init();

	Serial.println("****************************************************************");
	Serial.println("*************************** Setup OK ***************************"); // Everything's up and running
	Serial.println("****************************************************************\n");
}


// ------------------------------------- LOOP --------------------------------------

void loop() {
	// Grab and parse the OSC message from the Serial
	// wifi.cpp - osc.cpp
	wifi_.receive(osc_);

	// Find the laser which matches the osc message
	for (int i = 0; i < 4; i++) {
		if (osc_.getIp() == laser_[i].getIp() || osc_.getIp() == MASTER_IP) {
			// Update the motion values with the incoming message
			// motion.cpp - osc.cpp
			motion_[i].update(osc_);

			// Move according to the motion calculated and save it
			// laser.cpp - motion.cpp
			laser_[i].move(motion_[i]);
		}
	}

	// clean the osc message
	osc_.clean();
}
