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

#include <stddef.h>

#include <bell/melodies.h>

class Buzzer {
private:
	enum bzr_stat {
		UNINITIALIZED,
		IDLE,
		RINGING
	};

	uint8_t pin;
	note_t *melody;
        size_t melody_len;
	size_t i_tone;

	bzr_stat stat;
	unsigned long tstamp;

public:
	Buzzer();
	Buzzer(uint8_t pin, const note_t mel[], size_t melody_len);
	void ring();
	bool ringing();
	void update();
};