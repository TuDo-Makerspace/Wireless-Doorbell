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

#ifndef AP_SSID
#error No AP SSID specified! Please define AP_SSID in the build flags!
#endif

#ifndef AP_PWD
#error No AP password specified! Please define AP_PWD in the build flags!
#endif

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <log.h>
#include <Beacon.h>
#include <StatusLED.h>

#define REV "1.0.0"

#define CONNECTION_LED D0
#define POWER_LED D1
#define POWER D2

#define INDICATOR_LIGHTS_BLINK_INTERVAL 250 //ms
#define AP_ERR_BLINKS 5
#define CONN_TIMEOUT 20000 //ms (Should be at least 2x HOST_BEACON_TIMEOUT)
#define CONN_TIMEOUT_BLINKS 3
#define CONN_SUCCESS_TIME 2000 //ms

#define LATCH_POWER() digitalWrite(POWER, HIGH)
#define UNLATCH_POWER() digitalWrite(POWER, LOW)

Beacon *beacon;
StatusLED *power_led;
StatusLED *conn_led;

BeaconStatus stat;

void boot_msg()
{
        log_msg("Boot", "___       __   __"); 
        log_msg("Boot", " |  |  | |  \\ /  \\");
        log_msg("Boot", " |  \\__/ |__/ \\__/");                
        log_msg("Boot", "");                                     
        log_msg("Boot", " __   __   __   __   __   ___");     
        log_msg("Boot", "|  \\ /  \\ /  \\ |__) |__) |__  |    |");
        log_msg("Boot", "|__/ \\__/ \\__/ |  \\ |__) |___ |___ |___");
        log_msg("Boot", "");
        log_msg("Boot", "Author:\t\tPatrick Pedersen");
        log_msg("Boot", "License:\tGPLv3");
        log_msg("Boot", "Build date:\t" + String(__DATE__));
        log_msg("Boot", "Revision:\t" + String(REV) + "_DOOR");
        log_msg("Boot", "Source code:\thttps://github.com/TU-DO-Makerspace/Wireless-Doorbell");
        log_msg("Boot", "Device type:\tDoor");
        DBG_LOG("Boot", "AP SSID:\t" + String(AP_SSID));
        DBG_LOG("Boot", "AP Password:\t" + String(AP_PWD));
        log_msg("Boot", "");
}

void setup()
{
        pinMode(POWER, OUTPUT);
        LATCH_POWER();
        DBG_LOG("Power", "Power latched");

        beacon = new Beacon(AP_SSID, AP_PWD, CONN_TIMEOUT);
        power_led = new StatusLED(POWER_LED, INDICATOR_LIGHTS_BLINK_INTERVAL);
        conn_led = new StatusLED(CONNECTION_LED, INDICATOR_LIGHTS_BLINK_INTERVAL);

        power_led->mode(ON);

        Serial.begin(115200);
        Serial.println();

        boot_msg();

        log_msg("Beacon", "Starting beacon");
        
        bool res = beacon->start();
        conn_led->mode(BLINK);

        if (res) {
                log_msg("Beacon", "Successfully created beacon with a timeout of " + String(CONN_TIMEOUT) + "s");
                stat = ACTIVE;
        } else {
                power_led->mode(BLINK);
                log_msg("Beacon", "Failed to create beacon");

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
                        log_msg("Beacon", "No device connected, timing out");
                }
                else if (beacon->status() == SPOTTED) {
                        conn_led->mode(ON);
                        beacon->stop();
                        stat = SPOTTED;
                        log_msg("Beacon", "Beacon spotted");
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
                conn_led->mode(OFF);
                UNLATCH_POWER();
                static bool ultach_logged = false;
                if (!ultach_logged) {
                        log_msg("Power", "Power unlatched");
                        ultach_logged = true;
                }
        }
}

#endif