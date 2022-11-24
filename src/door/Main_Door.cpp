/*
 * Copyright (C) 2022 Patrick Pedersen, TU-DO Makerspace

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

/**
 * @file Main_Door.cpp
 * @author Patrick Pedersen
 * 
 * @brief Main file for the door firmware.
 * 
 * The following file contains the setup and loop function for the door firmware.
 * In the setup function, the power is latched using the P-MOS power latch
 * circuit. After the power has been latched, a Door object is created 
 * and initialized through a DoorCFG object. The run() method of the Door
 * object is then called continuously in the loop function to run the firmware.
 * 
 */

#ifdef TARGET_DEV_DOOR

#include <config.h>

#include <log.h>

#include <door/power_latch.h>
#include <door/Door.h>

Door door;

void setup()
{
	// Latch power ASAP before capacitor charges to P-MOSES threshold voltage
	LATCH_POWER();

	// Phew, we're safe here, now to the rest of the firmware

	Serial.begin(115200);

	log_msg("setup", "Power latched!");

	// Configure door using the DoorCFG object
	// Most parameters are set in the config.h file
	
	DoorCFG cfg;

	cfg.ring_led_pin 	= DOOR_RING_LED;
	cfg.power_led_pin 	= DOOR_POWER_LED;
	cfg.n_bells 		= DOOR_N_BELLS;
	cfg.ssid 		= WIFI_SSID;
	cfg.psk 		= WIFI_PSK;
	cfg.static_ip 		= DOOR_IP;
	cfg.gateway 		= GATEWAY;
	cfg.subnet 		= "255.255.255.0";
	cfg.port 		= TCP_PORT;
	cfg.con_timeout_s 	= DOOR_CONNECT_TIMEOUT_S;
	cfg.bell_timeout_ms 	= DOOR_BELL_TCP_TIMEOUT_MS;

	door = Door(cfg);
}

void loop()
{
	door.run();
}

#endif