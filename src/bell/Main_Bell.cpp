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

#ifdef TARGET_DEV_BELL

#include <log.h>
#include <config.h>

#include <bell/Bell.h>

Bell bell;

void setup()
{
	Serial.begin(115200);

	BellCFG cfg;

	cfg.buzzer_pin		= BELL_BUZZER;
	cfg.led_pin		= BELL_LED;
	cfg.ssid 		= WIFI_SSID;
	cfg.psk 		= WIFI_PSK;
	cfg.door_ip 		= DOOR_IP;
	cfg.static_ip 		= BELL_IP;
	cfg.gateway 		= GATEWAY;
	cfg.subnet 		= "255.255.255.0";
	cfg.port 		= TCP_PORT;

	bell = Bell(cfg);
}

void loop()
{
	bell.run();
}

#endif