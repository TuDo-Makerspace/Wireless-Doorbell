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
 * @file fallback_error.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief fallback_error for the Bell firmware
 */

#pragma once

#include <Arduino.h>

#include <log.h>
#include <config.h>

/**
 * @brief Bell Fallback error
 * 
 * The following function is called when the Bell class
 * encounters an error and has no possibility to display
 * it to the user via the LED (eg. no LED pin provided in
 * the class configuration).
 * 
 * The fallback error has direct access to the LED and
 * buzzer pins, and will blink the LED and play a A4
 * tone on the buzzer indefinitely.
 * 
 * @attention This function runs in an infinite loop!
*/
inline void FALLBACK_ERROR()
{
	log_msg("FALLBACK_ERROR", "Entered fallback error mode, something went horribly wrong!");

	pinMode(BELL_LED, OUTPUT);
	pinMode(BELL_BUZZER, OUTPUT);

	while(true) {
#ifndef BELL_SILENT
		tone(BELL_BUZZER, NOTE_A4);
#endif
		digitalWrite(BELL_LED, HIGH);
		delay(250);
		noTone(BELL_BUZZER);
		digitalWrite(BELL_LED, LOW);
		delay(250);
	}
}
