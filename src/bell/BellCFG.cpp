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

#include <bell/Bell.h>

bool BellCFG::checkValidity()
{
	bool ret = true;

	if (buzzer_pin == -1) {
		log_msg("BellCFG::valid", "Buzzer pin not specified in cfg!");
		ret = false;
	}

	if (led_pin == -1) {
		log_msg("BellCFG::valid", "LED pin not specified in cfg!");
		ret = false;
	}

	if (ssid == "") {
		log_msg("BellCFG::valid", "No SSID specified in cfg!");
		ret = false;
	}

	if (door_ip == "") {
		log_msg("BellCFG::valid", "No door IP specified in cfg!");
		ret = false;
	}

	// psk is not mandatory

	if (static_ip == "") {
		log_msg("BellCFG::valid", "No static IP specified in cfg!");
		ret = false;
	}
	
	if (gateway == "") {
		log_msg("BellCFG::valid", "No gateway specified in cfg!");
		ret = false;
	}

	if (subnet == "") {
		log_msg("BellCFG::valid", "No subnet specified in cfg!");
		ret = false;
	}

	if (port == 0) {
		log_msg("BellCFG::valid", "No port specified in cfg!");
		ret = false;
	}

	return ret;
}

#endif