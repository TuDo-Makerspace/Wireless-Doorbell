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

#ifndef SCAN_AP_SSID
#error No targeted SSID specified! Please define SCAN_AP_SSID in the build flags!
#endif

#ifndef SCAN_AP_PWD
#error No targeted password specified! Please define SCAN_AP_PWD in the build flags!
#endif

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <log.h>
#include <common.h>
#include <StatusLED.h>
#include <Scanner.h>
#include <Bell.h>
#include <Beacon.h>

#define REV "1.0.0"

#define BELL_LED D1
#define BUZZER D2

#define AP_ERR_BLINKS 5
#define HOST_BEACON_TIMEOUT 20000 //ms
#define CONN_TIMEOUT_BLINKS 3
#define CONN_SUCCESS_TIME 2000 //ms
#define BELL_LED_BLINK_INTERVAL NOTE_DURATION //ms
#define BELL_MELODY MEGALOVANIA // See melodies.h

bool ring_detected(Scanner *scanner)
{
        static bool prev_scan = false;
        bool ret = false;
        bool curr_scan = scanner->scan();
        
        ret = (!prev_scan && curr_scan);
        prev_scan = curr_scan;

        return ret;
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
                else if (led->blinks() == bell->melody_len/2) {
                        blink_complete = true;
                        led->mode(OFF);
                } else {
                        led->update();
                }
        }

        return !(bell_complete && blink_complete);
}

bool forward_beacon(Beacon *bcn)
{
        static bool req_bcn_stop = false;
        static unsigned long bcn_stop_tstamp;

        if (req_bcn_stop) {
                if (millis() >= bcn_stop_tstamp) {
                        bcn->stop();
                        req_bcn_stop = false;
                        return false;
                }

                return true;
        }

        BeaconStatus stat = bcn->status();

        if (stat == INACTIVE) {
                if (bcn->start())
                        log_msg("Beacon", "Starting forwarding beacon");
                else
                        log_msg("Beacon", "Failed to start forwarding beacon");
        }
        else if (stat == SPOTTED || stat == TIMEOUT) {
                if (stat == SPOTTED) {
                        log_msg("Beacon", "Beacon spotted");
                        req_bcn_stop = true;
                        bcn_stop_tstamp = millis() + MIN_SPOT_TIME;
                }
                else {
                        log_msg("Beacon", "No device connected, timing out");
                        req_bcn_stop = true;
                        bcn_stop_tstamp = millis();
                }
        }

        return true;
}

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
        log_msg("Boot", "Revision:\t" + String(REV) + "_BELL");
        log_msg("Boot", "Source code:\thttps://github.com/TU-DO-Makerspace/Wireless-Doorbell");
        log_msg("Boot", "Device type:\tBell");
        DBG_LOG("Boot", "Scan AP SSID:\t" + String(SCAN_AP_SSID));
        DBG_LOG("Boot", "Scan AP Password:\t" + String(SCAN_AP_PWD));
        
#ifdef HOST_AP_SSID
        DBG_LOG("Boot", "Host AP SSID:\t" + String(HOST_AP_SSID));
        DBG_LOG("Boot", "Host AP Password:\t" + String(HOST_AP_PWD));
#endif

        log_msg("Boot", "");
}

#ifdef HOST_AP_SSID
Beacon *beacon;
#endif

Scanner *scanner;

Bell *bell;
BellLED_t *led;

void setup()
{
#ifdef HOST_AP_SSID
        beacon = new Beacon(HOST_AP_SSID, HOST_AP_PWD, HOST_BEACON_TIMEOUT);
#endif
        scanner = new Scanner(SCAN_AP_SSID, SCAN_AP_PWD);
        bell = new Bell(BUZZER, BELL_MELODY, MELODY_LEN(BELL_MELODY));
        led = new BellLED_t(BELL_LED, BELL_LED_BLINK_INTERVAL);

        Serial.begin(115200);
        Serial.println();

        boot_msg();

        scanner->start();
        log_msg("Scanner", "Started scanner");
}

bool ring = false;
bool bell_complete = false;
bool beacon_complete = false;

void loop()
{
        if (!ring && ring_detected(scanner)) {
                ring = true;
                bell_complete = false;
                beacon_complete = false;
                log_msg("Scanner", "Detected ring signal");
#ifdef HOST_AP_SSID
                scanner->stop();
#endif
        }

        if (ring) {
#ifdef HOST_AP_SSID
                if (!beacon_complete && !forward_beacon(beacon)) {
                        scanner->start();
                        beacon_complete = true;
                }
#endif
                if (!bell_complete && !ring_bell(bell, led))
                        bell_complete = true;

#ifdef HOST_AP_SSID
                if (beacon_complete && bell_complete)
                        ring = false;
#else
                if (bell_complete)
                        ring = false;
#endif
        }
}

#endif