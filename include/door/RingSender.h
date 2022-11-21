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

#include <inttypes.h>

#include <door/RingTX.h>

class RingSender {
public:
	enum ring_stat {
		UNINITIALIZED,
		AWAITING,
		SENDING,
		SUCCESS,
		PARTIAL_SUCCESS,
		FAIL
	};

private:
	uint8_t n_bells;
	RingTX* tx;

	ring_stat stat;

public:
	RingSender();
	RingSender(IPAddress door_ip, unsigned int port, uint8_t n_bells, unsigned long timeout_ms);
	~RingSender();

	RingSender& operator=(const RingSender& other);

	uint8_t acks();
	uint8_t fails();

	void send();
	void update();
	
	ring_stat status();
};