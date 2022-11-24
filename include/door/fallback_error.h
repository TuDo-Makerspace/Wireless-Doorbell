/*
 * Copyright (C) 2022 Patrick Pedersen, TU-DO Makerspace

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

/**
 * @file fallback_errror.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief fallback_errror for the Door firmware
 */

#pragma once

#include <ESP8266WiFi.h>

#include <config.h>

#include <door/power_latch.h>

/**
 * @brief Door Fallback error
 * 
 * The following function is called when the Door class
 * encounters an error and has no possibility to display
 * it to the user via the LEDs (eg. no LEDs pin provided
 * in the class configuration).
 * 
 * The fallback error has direct access to the LEDs, and 
 * will blink each LED twice in a row. Once complete, the
 * device is powered off.
 * 
 */
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
