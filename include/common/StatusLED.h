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

#include <inttypes.h>

class StatusLED {
public:
        enum led_mode {
                OFF,
                ON,
                BLINK,
                BLINK_INV
        };

private:
        uint8_t pin;
        bool stat;
        led_mode mod;
        unsigned long blink_interval;
        unsigned long tstamp;
        unsigned int blks;
public:
        StatusLED();
        StatusLED(uint8_t pin, unsigned long blink_interval = 500);
        void setBlinkInterval(unsigned long interval);
        void mode(led_mode m);
        led_mode getMode();
        void update();
        unsigned int blinks();
};

typedef StatusLED BellLED_t;