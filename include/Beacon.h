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

#ifndef AP_SSID
#error No AP SSID specified! Please define AP_SSID in the build flags!
#endif

#ifndef AP_PWD
#error No AP password specified! Please define AP_PWD in the build flags!
#endif

#define CONN_TIMEOUT 10 //s

enum BeaconStatus {
        ACTIVE,
        INACTIVE,
        SPOTTED,
        TIMEOUT
};

class Beacon {
        unsigned long tout;
        bool running = false;
public:
        bool start();
        BeaconStatus status();
        void stop();
};