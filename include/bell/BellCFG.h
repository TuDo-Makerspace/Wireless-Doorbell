#pragma once

#include <WString.h>

class BellCFG {
public:
	int16_t buzzer_pin = -1;
	int16_t led_pin = -1;
	String ssid = "";
	String psk = "";
	String door_ip = "";
	String static_ip = "";
	String gateway = "";
	String subnet = "";
	uint16_t port = 0;

	bool checkValidity();
};