/*
 * Copyright (C) 2022 Patrick Pedersen, TUDO Makerspace

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
 * @file Main_Bell.cpp
 * @author Patrick Pedersen
 * 
 * @brief Main file for the bell firmware.
 * 
 * The following file contains the setup and loop function for the bell firmware.
 * In the setup function, a Bell object is created and initialized through a BellCFG
 * object. The run() method of the Bell object is then called continuously in the loop
 * function to run the firmware.
 * 
 */

#ifdef TARGET_DEV_BELL

#include <log.h>
#include <config.h>

#include <bell/Bell.h>

Bell bell;

void setup()
{
	Serial.begin(115200);

	// Configure door using the BellCFG object
	// Most parameters are set in the config.h file

	BellCFG cfg;

	cfg.buzzer_pin		= BELL_BUZZER;
	cfg.led_pin		= BELL_LED;
	cfg.ssid 		= WIFI_SSID;
	cfg.psk 		= WIFI_PSK;
	cfg.door_ip 		= DOOR_IP;
	cfg.static_ip 		= BELL_IP;
	cfg.gateway 		= GATEWAY;
	cfg.subnet 		= "255.255.255.0";
	cfg.port 		= TCP_PORT;

	bell = Bell(cfg);
}

void loop()
{
	bell.run();
}

#endif