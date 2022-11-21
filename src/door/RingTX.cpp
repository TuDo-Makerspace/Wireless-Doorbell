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

#include <log.h>
#include <ring_msg.h>

#include <door/RingTX.h>

RingTX::RingTX()
{
	stat = UNINITIALIZED;
}

RingTX::RingTX(String dest_ip, unsigned int port, unsigned long timeout_ms)
: ip(dest_ip), port(port), timeout(timeout_ms)
{
	log_msg("RingTX::RingTX", "Initializing RingTX to " + ip + ":" + String(port));
	stat = AWAITING;
}

void RingTX::send()
{
	if (stat == UNINITIALIZED) {
		log_msg("RingTX(to:" + ip + ":" + String(port) + ")::send", 
			"RingTX not initialized, cannot send!");
		return;
	}

	log_msg("RingTX(to:" + ip + ":" + String(port) + ")::send", 
		"Attempting to connect to bell at " + ip + ":" + String(port));

	client.connect(ip.c_str(), port);
	stat = CONNECTING;
	tstamp = millis() + timeout;
}

bool RingTX::txRingMSG()
{
	const char msg = RING_MSG;
	client.add(&msg, sizeof(msg));
	bool ret = client.send();
	return ret;
}

RingTX::ring_stat RingTX::con()
{
	if (client.connected()) {
		log_msg("RingTX(to:" + ip + ":" + String(port) + ")::con", 
			"Connected to bell at " + ip + ":" + String(port));
		tstamp = millis() + timeout;
		return SENDING;
	}

	if (timeout && millis() >= tstamp) {
		log_msg("RingTX(to:" + ip + ":" + String(port) + ")::con", 
			"Failed to connect to bell at " + ip + ":" + String(port) + ", timed out!");
		return FAIL;
	}

	return CONNECTING;
}

RingTX::ring_stat RingTX::sen()
{
	if (txRingMSG())
		return SUCCESS;

	if (timeout && millis() >= tstamp) {
		log_msg("RingTX(to:" + ip + ":" + String(port) + ")::send",
			"Failed to send ring msg to bell at " + ip + ":" + String(port) + ", timed out!");
		return FAIL;
	}

	return SENDING;
}

void RingTX::update()
{
	switch (stat) {
		case CONNECTING:
			stat = con();
			break;
		case SENDING:
			stat = sen();
			break;
		default:
			break;
	}
}

RingTX::ring_stat RingTX::status()
{
	return stat;
}

#endif