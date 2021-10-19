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

#ifdef TARGET_DEV_BELL

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <StatusLED.h>
#include <Scanner.h>
#include <Bell.h>

#define BUZZER D0
#define BELL_LED D1

#define AP_ERR_BLINKS 5
#define CONN_TIMEOUT_BLINKS 3
#define CONN_SUCCESS_TIME 2000 //ms
#define BELL_LED_BLINK_INTERVAL NOTE_DURATION //ms
#define BELL_LED_BLINKS BELL_MELODY_LEN/2

void detect_rings(Scanner *scanner, unsigned int &inc)
{
        static bool prev_scan = scanner->scan();
        bool curr_scan = scanner->scan();
        if (!prev_scan && curr_scan)
                inc++;
        prev_scan = curr_scan;
}

bool ring_bell(Bell *bell, BellLED_t *led)
{
        static bool bell_complete = false;
        static bool blink_complete = false;
        
        if (bell_complete && blink_complete) {
                bell_complete = false;
                blink_complete = false;  
        }  

        if (!bell_complete && !bell->play()) {
                bell_complete = true;
        }
        
        if (!blink_complete) {
                if (led->getMode() != BLINK) {
                        led->mode(BLINK);
                }
                else if (led->blinks() == BELL_LED_BLINKS) {
                        blink_complete = true;
                        led->mode(OFF);
                } else {
                        led->update();
                }
        }

        return !(bell_complete && blink_complete);
}

Scanner *scanner;
Bell *bell;
BellLED_t *led;

void setup()
{
        scanner = new Scanner();
        bell = new Bell(BUZZER);
        led = new BellLED_t(BELL_LED, BELL_LED_BLINK_INTERVAL);

        Serial.begin(115200);
        Serial.println();

        scanner->start();
}

unsigned int rings = 0;

void loop()
{
        detect_rings(scanner, rings);
        if (rings > 0 && !ring_bell(bell, led))
                rings--;
}

#endif