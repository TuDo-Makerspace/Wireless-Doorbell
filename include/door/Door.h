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
		UNINITIALIZED,
		CFG_INVALID,
		NO_ERROR,
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