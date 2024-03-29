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
 * @file StatusLED.cpp
 * @author Patrick Pedersen
 * 
 * @brief StatusLED class implementation
 * 
 * The following file contains the implementation of the StatusLED class.
 * For more information on the class, see the header file.
 * 
 */

#include <Arduino.h>

#include <log.h>
#include <StatusLED.h>

// Refer to header for documentation
StatusLED::StatusLED()
{
}

// Refer to header for documentation
StatusLED::StatusLED(uint8_t pin, unsigned long blink_interval_ms) : pin(pin)
{
        log_msg("StatusLED::StatusLED", "Initializing StatusLED on pin " + String(pin));
        pinMode(pin, OUTPUT);
        setBlinkInterval(blink_interval_ms);
        mode(OFF);
}

// Refer to header for documentation
void StatusLED::setBlinkInterval(unsigned long interval)
{
        blink_interval = interval;
}

// Refer to header for documentation
void StatusLED::mode(led_mode m) {
        switch(m) {
                case OFF:
                        log_msg("StatusLED(PIN:" + String(pin) + ")::mode", "Setting mode to OFF");
                        stat = LOW;
                        break;
                case ON:
                        log_msg("StatusLED(PIN:" + String(pin) + ")::mode", "Setting mode to ON");
                        stat = HIGH;
                        break;
                case BLINK: // ON then OFF
                        log_msg("StatusLED(PIN:" + String(pin) + ")::mode", "Setting mode to BLINK");
                        stat = HIGH;
                        tstamp = millis() + blink_interval;
                        break;
                case BLINK_INV: // OFF then ON
                        log_msg("StatusLED(PIN:" + String(pin) + ")::mode", "Setting mode to BLINK_INV");
                        stat = LOW;
                        tstamp = millis() + blink_interval;
                        m = BLINK;
                        break;
                default:
                        break;
        }

        digitalWrite(pin, stat);
        mod = m;
        blks = 0;
}

// Refer to header for documentation
StatusLED::led_mode StatusLED::getMode()
{
        return mod;
}

// Refer to header for documentation
void StatusLED::update()
{
        unsigned long t;
        if (mod == BLINK && (t = millis()) >= tstamp) {
                stat = !stat;
                digitalWrite(pin, stat);
                tstamp = t + blink_interval;
                if (!stat)
                        blks++;
        }
}

// Refer to header for documentation
unsigned int StatusLED::blinks()
{
        return blks;
}