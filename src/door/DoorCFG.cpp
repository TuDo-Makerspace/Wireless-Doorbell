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
 * @file DoorCFG.cpp
 * @author Patrick Pedersen
 * 
 * @brief DoorCFG class implementation
 * 
 * The following file contains the implementation of the DoorCFG class.
 * For more information on the class, see the header file.
 * 
 */

#ifdef TARGET_DEV_DOOR

#include <log.h>

#include <door/DoorCFG.h>

// Refer to header for documentation
bool DoorCFG::checkValidity()
{
	bool ret = true;

	if (ring_led_pin == -1) {
		log_msg("DoorCFG::valid", "Ring LED pin not specified in cfg!");
		ret = false;
	}

	if (power_led_pin == -1) {
		log_msg("DoorCFG::valid", "Power LED pin not specified in cfg!");
		ret = false;
	}

	if (n_bells == 0) {
		log_msg("DoorCFG::valid", "No bells specified in cfg!");
		ret = false;
	}

	if (ssid == "") {
		log_msg("DoorCFG::valid", "No SSID specified in cfg!");
		ret = false;
	}

	if (static_ip == "") {
		log_msg("DoorCFG::valid", "No static IP specified in cfg!");
		ret = false;
	}

	if (gateway == "") {
		log_msg("DoorCFG::valid", "No gateway specified in cfg!");
		ret = false;
	}

	if (subnet == "") {
		log_msg("DoorCFG::valid", "No subnet specified in cfg!");
		ret = false;
	}

	if (port == 0) {
		log_msg("DoorCFG::valid", "No port specified in cfg!");
		ret = false;
	}

	return ret;
}

#endif