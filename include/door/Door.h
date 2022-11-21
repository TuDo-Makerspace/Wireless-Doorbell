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

#pragma once

#include <ESP8266WiFi.h>

#include <StatusLED.h>
#include <WiFiHandler.h>

#include <door/DoorCFG.h>
#include <door/RingSender.h>

class Door {
	enum door_state {
		INIT,
		CONNECTING,
		CONNECTED,
		RINGING,
		ERROR,
		ERROR_HANDLING,
		POWER_OFF,
		POWERED_OFF,
	};

	enum error_type {
		NO_ERROR,
		UNINITIALIZED,
		CFG_INVALID,
		NO_WIFI,
		PARTIAL_SUCCESS,
		FAIL,
	};

	DoorCFG cfg;

	StatusLED ring_led;
	StatusLED pwr_led;

	WiFiHandler wifi_handler;
	RingSender ring_sender;

	door_state state;
	error_type err;
	
	void bootMSG();

	door_state init();
	door_state connecting();
	door_state connected();
	door_state ringing();
	door_state error();
	door_state error_handling();
	door_state power_off();

public:
	Door();
	Door(DoorCFG cfg);
	void run();
};