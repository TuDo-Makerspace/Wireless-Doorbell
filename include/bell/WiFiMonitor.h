#pragma once

#include <StatusLED.h>

enum con_stat {
	NOT_CONNECTED = 0,
	CONNECTED = 1
};

class WiFiMonitor {
	con_stat prev_con_stat;
	StatusLED led;
	bool init = false;
	
	void onConnect();
	void onDisconnect();
	void onInit();
public:
	WiFiMonitor(uint8_t led_pin, unsigned long con_blink_interval_ms);
	void update();
};