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

#include <config.h>

#include <door/power_latch.h>

inline void FALLBACK_ERROR()
{
	log_msg("FALLBACK_ERROR", "Entered fallback error mode, something went horribly wrong!");

	pinMode(DOOR_POWER_LED, OUTPUT);
	pinMode(DOOR_RING_LED, OUTPUT);

	for (uint8_t i = 0; i < 2; i++) {
		digitalWrite(DOOR_POWER_LED, HIGH);
		delay(250);
		digitalWrite(DOOR_POWER_LED, LOW);
		delay(250);
	}

	for (uint8_t i = 0; i < 2; i++) {
		digitalWrite(DOOR_RING_LED, HIGH);
		delay(250);
		digitalWrite(DOOR_RING_LED, LOW);
		delay(250);
	}

	log_msg("FALLBACK_ERROR", "Unlatching power");

	UNLATCH_POWER();
}
