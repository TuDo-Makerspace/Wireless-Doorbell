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

#include <ESP8266WiFi.h>
#include <Scanner.h>

Scanner::Scanner(const char *ap_ssid, const char *ap_pwd)
{
        strcpy(ssid, ap_ssid);
        strcpy(pwd, ap_pwd);
}

void Scanner::start()
{
        WiFi.begin(ssid, pwd);
}

bool Scanner::scan()
{
        return (WiFi.status() == WL_CONNECTED);
}

void Scanner::stop()
{
        WiFi.disconnect(true);
}