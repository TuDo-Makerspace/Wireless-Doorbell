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