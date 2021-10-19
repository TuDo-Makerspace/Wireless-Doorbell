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

#ifdef TARGET_DEV_DOOR

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <Beacon.h>
#include <StatusLED.h>

#define CONNECTION_LED D0
#define POWER_LED D1
#define BTN_STAT D2

#define INDICATOR_LIGHTS_BLINK_INTERVAL
#define AP_ERR_BLINKS 5
#define CONN_TIMEOUT_BLINKS 3
#define CONN_SUCCESS_TIME 2000 //ms

Beacon *beacon;
StatusLED *power_led;
StatusLED *conn_led;

BeaconStatus stat;

void setup()
{

        beacon = new Beacon();
        power_led = new StatusLED(POWER_LED, INDICATOR_LIGHTS_BLINK_INTERVAL);
        conn_led = new StatusLED(CONNECTION_LED, INDICATOR_LIGHTS_BLINK_INTERVAL);
        
        power_led->mode(ON);

        pinMode(BTN_STAT, INPUT);

        Serial.begin(115200);
        Serial.println();

        Serial.println("Starting beacon...");
        bool res = beacon->start();
        conn_led->mode(BLINK);

        if (res) {
                Serial.println("Successfully created Beacon");
                stat = ACTIVE;
        } else {
                power_led->mode(BLINK);
                Serial.println("Failed to create Beacon");

                while(power_led->blinks() <= AP_ERR_BLINKS) {
                        power_led->update();
                        conn_led->update();
                }

                stat = INACTIVE;
        }
}

void loop()
{
        if (stat == ACTIVE) {
                if (beacon->status() == TIMEOUT) {
                        beacon->stop();
                        power_led->mode(BLINK);
                        conn_led->mode(BLINK);
                        stat = TIMEOUT;
                        Serial.println("Beacon timed out!");
                }
                else if (beacon->status() == SPOTTED) {
                        conn_led->mode(ON);
                        beacon->stop();
                        stat = SPOTTED;
                        Serial.println("Beacon spotted!");
                }

                power_led->update();
                conn_led->update();
        }

        if (stat == TIMEOUT) {
                if (power_led->blinks() <= CONN_TIMEOUT_BLINKS) {
                        power_led->update();
                        conn_led->update();
                } else {
                        stat = INACTIVE;
                }
        }
        else if (stat == SPOTTED) {
                delay(CONN_SUCCESS_TIME);
                stat = INACTIVE;
        }
        
        if (stat == INACTIVE) {
                power_led->mode(OFF);
                conn_led->mode(OFF);

                if (!digitalRead(digitalRead(BTN_STAT))) {
                        void; // Power off
                }
        }
}

#endif