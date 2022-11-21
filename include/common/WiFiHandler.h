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

class WiFiHandler {
public:
	enum wifi_stat {
		UNINITIALIZED,
		DISCONNECTED,
		CONNECTING,
		CONNECTED
	};

private:
	String ssid;
	String psk;
	IPAddress ip;
	IPAddress gateway;
	IPAddress subnet;
	uint16_t timeout_ms;
	bool rejoin;

	wifi_stat stat = DISCONNECTED;
	unsigned long timeout_tstamp;

	bool timeout();
	void _connect();
	
	void unexpected_disconnect();

public:
	WiFiHandler();
	WiFiHandler(const String ssid, const String psk, 
		    const IPAddress ip, const IPAddress gateway, const IPAddress subnet,
		    const uint16_t timeout_s, const bool rejoin = true);

	void connect();
	void disconnect();
	void update();
	wifi_stat status();
};