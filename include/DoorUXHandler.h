#ifdef TARGET_DEV_DOOR

#pragma once

#include <StatusLED.h>

class DoorUXHandler {
	
	enum req {
		IDLE = 0,
		COMPLETE = 0,
		REQUEST = 1,
		HANDLE = 2
	};

	enum door_ux_err {
		NO_WIFI,
		PARTIAL_SUCCESS,
		FAIL,
	};

	uint8_t n_bells;
	uint8_t bells_remaining;

	StatusLED ring_led;
	StatusLED pwr_led;

	bool init = false;
	bool _done = false;
	uint8_t successfull_connections = 0;
	
	req err_req = IDLE;
	door_ux_err err_type;

	void onFirstBellAck();
	void onLastDisconnect();
	
	void handleError();
public:
	DoorUXHandler(uint8_t n_bells, uint8_t ring_led_pin, uint8_t pwr_led_pin);
	void bellAcknowledged();
	void bellDisconnected();
	void wifiError();
	bool done();
	void update();
};

#endif