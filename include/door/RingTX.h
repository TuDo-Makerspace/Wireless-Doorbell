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

#include <Arduino.h>
#include <ESPAsyncTCP.h>

class RingTX {
public:
	enum ring_stat {
		UNINITIALIZED,
		AWAITING,
		CONNECTING,
		SENDING,
		SUCCESS,
		FAIL
	};

private:
	String ip;
	unsigned int port;
	AsyncClient client;
	unsigned long timeout;
	unsigned long tstamp;
	
	ring_stat stat = UNINITIALIZED;

	ring_stat con();
	ring_stat sen();

public:
	RingTX();
	RingTX(String dest_ip, unsigned int port, unsigned long timeout_ms);

	void send();
	void update();

	bool txRingMSG();

	ring_stat status();
};