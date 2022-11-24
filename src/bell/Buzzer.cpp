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

/**
 * @file Buzzer.cpp
 * @author Patrick Pedersen
 * 
 * @brief Buzzer class implementation
 * 
 * The following file contains the implementation of the Buzzer class.
 * For more information on the class, see the header file.
 * 
 */

#ifdef TARGET_DEV_BELL

#include <Arduino.h>

#include <log.h>
#include <config.h>

#include <bell/Buzzer.h>

// Refer to header for documentation
Buzzer::Buzzer()
{
	stat = UNINITIALIZED;
}

// Refer to header for documentation
Buzzer::Buzzer(uint8_t pin, const note_t mel[], size_t melody_len) : pin(pin), melody_len(melody_len)
{
	// Melody array is a const, so we need to copy it to a non-const array
	// if we wish to read it from within the class.
	// We can theoretically afford to just work unsafe here, since
	// we will only ever read from the melody array, let alone
	// never use more than one Buzzer. However, since we still have
	// plenty of memory, we might as well just do it the "right way".

	melody = new note_t[melody_len];
        memcpy(melody, mel, melody_len * sizeof(note_t));

	pinMode(pin, OUTPUT);
	stat = IDLE;
}

// Refer to header for documentation
void Buzzer::ring()
{
	if (stat == UNINITIALIZED) {
		log_msg("Buzzer(PIN:" + String(pin) + ")::ring", "Attempted to ring with uninitialized Buzzer!");
		return;
	}

	if (stat == RINGING) {
		log_msg("Buzzer(PIN:" + String(pin) + ")::ring", "Attempted to ring while already ringing!");
		return;
	}

	log_msg("Buzzer(PIN:" + String(pin) + ")::ring", "Ringing!");

	i_tone = 0;
	tstamp = millis() + NOTE_DURATION;
	stat = RINGING;
}

// Refer to header for documentation
bool Buzzer::ringing()
{
	return stat == RINGING;
}

// Refer to header for documentation
void Buzzer::update()
{
	if (stat != RINGING)
		return;

	if (millis() < tstamp)
		return;

	if (i_tone == melody_len) {
		noTone(pin);
		stat = IDLE;
		log_msg("Buzzer::update", "Done ringing!");
		return;
	}

#ifndef BELL_SILENT
	tone(pin, melody[i_tone]);
#endif

	i_tone++;
	tstamp = millis() + NOTE_DURATION;
}

#endif