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