#pragma once

#include <WString.h>

class DoorCFG {
public:
	int16_t ring_led_pin = -1;
	int16_t power_led_pin = -1;
	uint16_t con_timeout_s = 0;
	uint8_t n_bells = 0;
	String ssid = "";
	String psk = "";
	String static_ip = "";
	String gateway = "";
	String subnet = "";
	uint16_t port = 0;
	unsigned long bell_timeout_ms = 0;

	bool checkValidity();
};