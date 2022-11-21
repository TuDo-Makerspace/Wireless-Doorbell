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

#ifdef TARGET_DEV_DOOR

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <log.h>
#include <door/RingSender.h>

RingSender::RingSender()
{
	stat = UNINITIALIZED;
}

RingSender::RingSender(IPAddress door_ip, unsigned int port, uint8_t n_bells, unsigned long timeout_ms)
: n_bells(n_bells)
{
	log_msg("RingSender::RingSender", "Initializing RingSender");
	
	const String door_network_address = String(door_ip[0]) + "." + String(door_ip[1]) + "." + String(door_ip[2]);
	const uint8_t door_host_id = door_ip[3];

	tx = new RingTX[n_bells];

	for (uint8_t i = 0; i < n_bells; i++) {
		String dest_ip = door_network_address + "." + String((door_host_id + 1) + i);
		tx[i] = RingTX(dest_ip, port, timeout_ms);
	}

	stat = AWAITING;
}

RingSender::~RingSender()
{
	if (stat == UNINITIALIZED)
		return;
	
	delete[] tx;
}

RingSender& RingSender::operator=(const RingSender& other)
{
	if (this == &other)
		return *this;

	n_bells = other.n_bells;
	stat = other.stat;
	
	tx = new RingTX[n_bells];
	for (uint8_t i = 0; i < n_bells; i++)
		tx[i] = other.tx[i];

	return *this;
}

uint8_t RingSender::acks() {
	uint8_t ret = 0;
	for (int i = 0; i < n_bells; i++) {
		if (tx[i].status() == RingTX::SUCCESS)
			ret++;
	}

	return ret;
}

uint8_t RingSender::fails() {
	uint8_t ret = 0;
	for (int i = 0; i < n_bells; i++) {
		if (tx[i].status() == RingTX::FAIL)
			ret++;
	}

	return ret;
}

void RingSender::send()
{
	if (stat == UNINITIALIZED) {
		log_msg("RingSender::send", "RingSender not initialized, cannot send!");
		return;
	}

	log_msg("RingSender::send", "Sending ring msg to " + String(n_bells) + " bells");

	for (uint8_t i = 0; i < n_bells; i++) {
		tx[i].send();
	}

	stat = SENDING;
}

void RingSender::update()
{
	if (stat != SENDING)
		return;

	for (uint8_t i = 0; i < n_bells; i++) {
		tx[i].update();
	}

	uint8_t _acks = acks();
	uint8_t _fails = fails();
	uint8_t total = _acks + _fails;

	if (total < n_bells)
		return;
	
	if (_fails == n_bells) {
		log_msg("RingSender::update", 
			"Failed to send ring msg to all " + String(n_bells) + " bells");
		stat = FAIL;
	}
	else if (_acks == n_bells) {
		log_msg("RingSender::update",
			"Successfully sent ring msg to all " + String(n_bells) + "/" + String(n_bells) + " bells");
		stat = SUCCESS;
	} else {
		log_msg("RingSender::update", 
			"Failed to send ring msg to " + String(_fails) + "/" + String(n_bells) + " bells");
		stat = PARTIAL_SUCCESS;
	}
}

RingSender::ring_stat RingSender::status()
{
	return stat;
}

#endif