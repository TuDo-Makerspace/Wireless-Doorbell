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

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <Beacon.h>

bool Beacon::start()
{
        bool ret = WiFi.softAP(AP_SSID, AP_PWD);
        tout = millis() + (CONN_TIMEOUT * 1000);
        running = true;
        return ret;
}

BeaconStatus Beacon::status()
{
        if (millis() >= tout)
                return TIMEOUT;
        
        if (wifi_softap_get_station_info() != NULL) {
                wifi_softap_free_station_info();
                return SPOTTED;
        }

        return running ? ACTIVE : INACTIVE;
}

void Beacon::stop()
{
        running = false;
        WiFi.softAPdisconnect(true);
}

