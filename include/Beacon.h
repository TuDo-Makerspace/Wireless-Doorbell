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

#include <ESP8266WiFi.h>
#include <StatusLED.h>

#define MAX_SSID_LEN 32
#define MAX_PWD_LEN 63

enum BeaconStatus {
        ACTIVE,
        INACTIVE,
        SPOTTED,
        TIMEOUT
};

class Beacon {
        char ssid[MAX_SSID_LEN];
        char pwd[MAX_PWD_LEN];
        unsigned long timeout;
        
        unsigned long tstamp;
        bool running = false;
public:
        Beacon(const char *ap_ssid, const char *ap_pwd, unsigned long timeout);
        bool start();
        BeaconStatus status();
        void stop();
};