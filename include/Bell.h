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

#include <pitches.h>

#define NOTE_DURATION 50 //ms

const unsigned int BELL_MELODY[] = {
        NOTE_D7, NOTE_D7, NOTE_D7, NOTE_PAUSE,
        NOTE_D7, NOTE_D7, NOTE_D7, NOTE_PAUSE,
        NOTE_D8, NOTE_D8, NOTE_D8, NOTE_D8,
        NOTE_D8, NOTE_D8, NOTE_PAUSE, NOTE_PAUSE,
        NOTE_A7, NOTE_A7, NOTE_A7, NOTE_A7, 
        NOTE_A7, NOTE_A7, NOTE_PAUSE, NOTE_PAUSE,
        NOTE_PAUSE, NOTE_PAUSE, NOTE_PAUSE, NOTE_PAUSE, 
        NOTE_GS7, NOTE_GS7, NOTE_GS7, NOTE_GS7, 
        NOTE_PAUSE, NOTE_PAUSE, NOTE_PAUSE, NOTE_PAUSE, 
        NOTE_G7, NOTE_G7, NOTE_G7, NOTE_G7, 
        NOTE_PAUSE, NOTE_PAUSE, NOTE_PAUSE, NOTE_PAUSE, 
        NOTE_F7, NOTE_F7, NOTE_F7, NOTE_F7, 
        NOTE_F7, NOTE_F7, NOTE_F7, NOTE_PAUSE,
        NOTE_D7, NOTE_D7, NOTE_D7, NOTE_PAUSE,
        NOTE_F7, NOTE_F7, NOTE_F7, NOTE_PAUSE,
        NOTE_G7, NOTE_G7, NOTE_G7
};

#define BELL_MELODY_LEN sizeof(BELL_MELODY) / sizeof(unsigned int)

class Bell {
        uint8_t pin;
        int curr_tone = -1;
        unsigned long tstamp;

public:
        Bell(uint8_t pin);
        bool play();
};