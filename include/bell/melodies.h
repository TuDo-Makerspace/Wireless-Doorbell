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

#include <bell/pitches.h>

#define NOTE_DURATION 50 //ms

static const note_t DEFAULT_CHIME[] = {
        NOTE_D7, NOTE_FS7, NOTE_A7,
        NOTE_D7, NOTE_FS7, NOTE_A7,
        NOTE_D7, NOTE_FS7, NOTE_A7,
        NOTE_D7, NOTE_FS7, NOTE_A7,
        NOTE_D7, NOTE_FS7, NOTE_A7,
        NOTE_D7, NOTE_FS7, NOTE_A7,
        NOTE_D7, NOTE_FS7, NOTE_A7,
        NOTE_D7, NOTE_FS7, NOTE_A7,
        NOTE_D7, NOTE_FS7, NOTE_A7
};

static const note_t DEBUG_CHIME[] = {
        NOTE_A4, NOTE_A4, NOTE_A4,
        NOTE_PAUSE, NOTE_PAUSE, NOTE_PAUSE,
        NOTE_A4, NOTE_A4, NOTE_A4
};

static const note_t MEGALOVANIA[] = {
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

#define MELODY_LEN(MELODY) (sizeof(MELODY) / sizeof(note_t))
