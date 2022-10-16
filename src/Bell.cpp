#ifdef TARGET_DEV_BELL

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

#include <string.h>

#include <Arduino.h>
#include <Bell.h>

#include "config.h"

Bell::Bell(uint8_t bzr_pin, uint8_t led_pin, const note_t mel[], size_t melody_len) 
: bzr_pin(bzr_pin), led(StatusLED(led_pin)), melody_len(melody_len)
{
        melody = new note_t[melody_len];
        memcpy(melody, mel, melody_len * sizeof(note_t));
        pinMode(bzr_pin, OUTPUT);
        tstamp = millis();
}

bool Bell::play()
{
        bool curr_tone_init = (curr_tone > -1);
        bool t_passed = (millis() >= tstamp);

        if (!curr_tone_init || t_passed) {
                curr_tone++;
                
                if ((size_t)curr_tone == melody_len) {
                        curr_tone = -1;
                        tone(bzr_pin, 0);
                        led.mode(OFF);
                        return false;
                }
                
                tstamp = millis() + NOTE_DURATION;
#ifndef BELL_SILENT
                tone(bzr_pin, melody[curr_tone]);
#endif
        }

        return true;
}

bool Bell::ring()
{
        if (ringing)
                return false;

        led.mode(ON);
        ringing = true;
        return true;
}

void Bell::update()
{
        if (ringing)
                ringing = play();

        led.update();
}

#endif