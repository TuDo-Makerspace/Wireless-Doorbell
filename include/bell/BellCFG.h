/*
 * Copyright (C) 2022 Patrick Pedersen, TU-DO Makerspace

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

/**
 * @file BellCFG.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief BellCFG class
 */

#pragma once

#include <WString.h>

/**
 * @brief The BellCFG class.
 * 
 * The BellCFG class is used to configure a Bell class instance and
 * provides a function to check its validity (see checkValidity()).
 */
class BellCFG {
public:
	int16_t buzzer_pin = -1;
	int16_t led_pin = -1;
	String ssid = "";
	String psk = "";
	String door_ip = "";
	String static_ip = "";
	String gateway = "";
	String subnet = "";
	uint16_t port = 0;

	/**
	 * @brief Checks if the configuration is valid
	 * 
	 * The following function checks if the configuration is valid.
	 * If one of the required fields is not set, or set improperly, the
	 * function returns false.
	 * 
	 * @returns true If the configuration is valid,
	 * 	    false If the configuration is invalid
	 */
	bool checkValidity();
};