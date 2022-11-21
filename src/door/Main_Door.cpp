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

#ifdef TARGET_DEV_DOOR

#include <config.h>

#include <log.h>

#include <door/power_latch.h>
#include <door/Door.h>

Door door;

void setup()
{
	LATCH_POWER();

	Serial.begin(115200);

	log_msg("setup", "Power latched!");

	DoorCFG cfg;

	cfg.ring_led_pin 	= DOOR_RING_LED;
	cfg.power_led_pin 	= DOOR_POWER_LED;
	cfg.n_bells 		= DOOR_N_BELLS;
	cfg.ssid 		= WIFI_SSID;
	cfg.psk 		= WIFI_PSK;
	cfg.static_ip 		= DOOR_IP;
	cfg.gateway 		= GATEWAY;
	cfg.subnet 		= "255.255.255.0";
	cfg.port 		= TCP_PORT;
	cfg.con_timeout_s 	= DOOR_CONNECT_TIMEOUT_S;
	cfg.bell_timeout_ms 	= DOOR_BELL_TCP_TIMEOUT_MS;

	door = Door(cfg);
}

void loop()
{
	door.run();
}

#endif