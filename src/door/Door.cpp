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
 * @file Door.cpp
 * @author Patrick Pedersen
 * 
 * @brief Door class implementation
 * 
 * The following file contains the implementation of the Door class.
 * For more information on the class, see the header file.
 * 
 */

#ifdef TARGET_DEV_DOOR

#include <config.h>

#include <log.h>
#include <StatusLED.h>

#include <door/Door.h>
#include <door/fallback_error.h>
#include <door/power_latch.h>

// Refer to header for documentation
Door::Door()
{
	err = UNINITIALIZED;
	state = ERROR;
}

// Refer to header for documentation
Door::Door(DoorCFG door_cfg) : cfg(door_cfg)
{	
	log_msg("Door::Door", "Initializing door");

	if (!cfg.checkValidity()) {
		if (cfg.ring_led_pin == -1 ||
		    cfg.power_led_pin == -1) {
			log_msg("Door", "Status LEDs uninitialized, entering fallback error mode");
			FALLBACK_ERROR(); // Unlatches power after completion	
		}

		err = CFG_INVALID;
		state = ERROR;
		return;
	}

	ring_led = StatusLED(cfg.ring_led_pin);
	pwr_led = StatusLED(cfg.power_led_pin);

	IPAddress ip, gateway, subnet;
	ip.fromString(cfg.static_ip);
	gateway.fromString(cfg.gateway);
	subnet.fromString(cfg.subnet);

	wifi_handler = WiFiHandler(
		cfg.ssid, cfg.psk,
		ip, gateway, subnet,
		cfg.con_timeout_s, false
	);
	
	ring_sender = RingSender(ip, cfg.port, cfg.n_bells, cfg.bell_timeout_ms);

	state = INIT;
}

// Refer to header for documentation
void Door::bootMSG()
{
	log_msg("Door::bootMSG", "---------------------------------------------------------------------------");
	log_msg("Door::bootMSG", "___       __   __"); 
	log_msg("Door::bootMSG", " |  |  | |  \\ /  \\");
	log_msg("Door::bootMSG", " |  \\__/ |__/ \\__/");                
	log_msg("Door::bootMSG", "");                                     
	log_msg("Door::bootMSG", " __   __   __   __   __   ___");     
	log_msg("Door::bootMSG", "|  \\ /  \\ /  \\ |__) |__) |__  |    |");
	log_msg("Door::bootMSG", "|__/ \\__/ \\__/ |  \\ |__) |___ |___ |___");
	log_msg("Door::bootMSG", "");
	log_msg("Door::bootMSG", "Author:\t\t\tPatrick Pedersen");
	log_msg("Door::bootMSG", "License:\t\t\tGPLv3");
	log_msg("Door::bootMSG", "Build date:\t\t" + String(__DATE__));
	log_msg("Door::bootMSG", "Software Revision:\t" + String(SW_REV) + "_DOOR");
	log_msg("Door::bootMSG", "Hardware Revision:\t" + String(HW_REV) + "_DOOR");
	log_msg("Door::bootMSG", "Source code:\t\thttps://github.com/TU-DO-Makerspace/Wireless-Doorbell");
	log_msg("Door::bootMSG", "Device type:\t\tDoor");
	log_msg("Door::bootMSG", "Targeted SSID:\t\t" + String(WIFI_SSID));
	log_msg("Door::bootMSG", "---------------------------------------------------------------------------");
	log_msg("Door::bootMSG", "");
}

// Refer to header for documentation
Door::door_state Door::init()
{
	bootMSG();
	pwr_led.mode(StatusLED::ON);
	wifi_handler.connect();
	return CONNECTING;
}

// Refer to header for documentation
Door::door_state Door::connecting()
{	
	switch (wifi_handler.status()) {
		case WiFiHandler::CONNECTING: return CONNECTING;
		case WiFiHandler::CONNECTED:  return CONNECTED;
		default:
			err = NO_WIFI;
			return ERROR;
	}
}

// Refer to header for documentation
Door::door_state Door::connected()
{
	ring_sender.send();
	return RINGING;
}

// Refer to header for documentation
Door::door_state Door::ringing()
{	
	switch (ring_sender.status()) {
		case RingSender::SENDING: {
			if (ring_sender.acks() > 0 && 
			    ring_led.getMode() != StatusLED::ON) {
				ring_led.mode(StatusLED::ON);
			}
			return RINGING;
		}
		
		case RingSender::SUCCESS: {
			ring_led.mode(StatusLED::OFF);
			return POWER_OFF;
		}

		case RingSender::PARTIAL_SUCCESS: {
			ring_led.mode(StatusLED::OFF);
			err = PARTIAL_SUCCESS;
			return ERROR;
		}

		default: {
			err = FAIL;
			return ERROR;
		}
	}
}

// Refer to header for documentation
Door::door_state Door::error()
{
	switch (err) {
		case UNINITIALIZED:
			log_msg("Door::error", "Door not initialized!");
			FALLBACK_ERROR();
			break;
		case CFG_INVALID:
			log_msg("Door::error", "Invalid configuration provided!");
			pwr_led.setBlinkInterval(250);
			pwr_led.mode(StatusLED::BLINK);
			break;
		case NO_WIFI:
			log_msg("Door::error", "Failed to establish a WiFi connection!");
			pwr_led.mode(StatusLED::BLINK_INV);
			ring_led.mode(StatusLED::BLINK);
			break;
		case PARTIAL_SUCCESS:
			log_msg("Door::error", "Partial success, some bells did not ring!");
			pwr_led.mode(StatusLED::BLINK);
			ring_led.mode(StatusLED::BLINK);
			break;
		case FAIL:
			log_msg("Door::error", "Failed to contact bells!");
			pwr_led.mode(StatusLED::BLINK);
			break;
	}
	
	return ERROR_HANDLING;
}

// Refer to header for documentation
Door::door_state Door::error_handling()
{
	switch(err) {
		case CFG_INVALID: {
			if (pwr_led.blinks() >= DOOR_CFG_INVALID_BLINKS) {
				return POWER_OFF;
			}
			break;
		}

		case NO_WIFI: {
			if (pwr_led.blinks() >= DOOR_NO_WIFI_BLINKS &&
			    ring_led.blinks() >= DOOR_NO_WIFI_BLINKS) {
				return POWER_OFF;
			}
			break;
		}
		
		case PARTIAL_SUCCESS: {
			if (pwr_led.blinks() >= DOOR_PARTIAL_SUCCESS_BLINKS &&
			    ring_led.blinks() >= DOOR_PARTIAL_SUCCESS_BLINKS) {
				return POWER_OFF;
			}
			break;
		}

		case FAIL: {
			if (pwr_led.blinks() >= DOOR_NO_BELLS_BLINKS) {
				return POWER_OFF;
			}
			break;
		}

		default: {
			break;
		}
	}

	return ERROR_HANDLING;
}

// Refer to header for documentation
Door::door_state Door::power_off()
{
	log_msg("Door::power_off", "Unlatching power, shutting down...");

	pwr_led.mode(StatusLED::OFF);
	ring_led.mode(StatusLED::OFF);

	UNLATCH_POWER();
	return POWERED_OFF;
}

// Refer to header for documentation
void Door::run()
{
	switch (state) {
		case INIT:		state = init(); 		break;
		case CONNECTING:	state = connecting();		break;
		case CONNECTED:		state = connected();		break;
		case RINGING:		state = ringing();		break;
		case ERROR:		state = error();		break;
		case ERROR_HANDLING:	state = error_handling();	break;
		case POWER_OFF:		state = power_off();		break;
		case POWERED_OFF:					break;
	}

	wifi_handler.update();
	ring_sender.update();
	pwr_led.update();
	ring_led.update();
}

#endif