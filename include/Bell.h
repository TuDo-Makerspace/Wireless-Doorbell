/*
 * Copyright (C) 2021 Patrick Pedersen, TUDO Makerspace

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
#include <inttypes.h>

#include <melodies.h>
#include <StatusLED.h>

class Bell {
        uint8_t bzr_pin;
        StatusLED led;
        int curr_tone = -1;
        unsigned long tstamp;
        bool ringing = false;

        bool play();

public:
        note_t *melody;
        size_t melody_len;

        Bell(uint8_t bzr_pin, uint8_t led_pin, const note_t mel[], size_t len);
        bool ring();
        void update();
};