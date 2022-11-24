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

/**
 * @file Bell.cpp
 * @author Patrick Pedersen
 * 
 * @brief Bell class implementation
 * 
 * The following file contains the implementation of the Bell class.
 * For more information on the class, see the header file.
 * 
 */

#ifdef TARGET_DEV_BELL

#include <config.h>

#include <log.h>
#include <StatusLED.h>

#include <bell/fallback_error.h>
#include <bell/Bell.h>

// Refer to header for documentation
Bell::Bell()
{
	err = UNINITIALIZED;
	state = ERROR;
}

// Refer to header for documentation
Bell::Bell(BellCFG bell_cfg) : cfg(bell_cfg)
{
	log_msg("Bell::Bell", "Initializing bell");

	if (!cfg.checkValidity()) {
		if (cfg.led_pin == -1) {
			 // We can't display error codes if LED pin isn't initialized!
			 // Resort to a fallback error!
			log_msg("Bell::Bell", "LED indicator uninitialized, entering fallback error mode");
			FALLBACK_ERROR(); // Enters infinite loop
		}

		err = CFG_INVALID;
		state = ERROR;
		return;
	}

	led = StatusLED(cfg.led_pin);
	buzzer = Buzzer(cfg.buzzer_pin, BELL_MELODY, MELODY_LEN(BELL_MELODY));

	IPAddress ip, gateway, subnet;
	ip.fromString(cfg.static_ip);
	gateway.fromString(cfg.gateway);
	subnet.fromString(cfg.subnet);

	wifi_handler = WiFiHandler(
		cfg.ssid, cfg.psk,
		ip, gateway, subnet,
		0
	);

	ring_receiver = RingReceiver::get_instance();

	state = INIT;
}

// Refer to header for documentation
void Bell::bootMSG()
{
	log_msg("Bell::bootMSG", "---------------------------------------------------------------------------");
	log_msg("Bell::bootMSG", "___       __   __"); 
	log_msg("Bell::bootMSG", " |  |  | |  \\ /  \\");
	log_msg("Bell::bootMSG", " |  \\__/ |__/ \\__/");                
	log_msg("Bell::bootMSG", "");                                     
	log_msg("Bell::bootMSG", " __   __   __   __   __   ___");     
	log_msg("Bell::bootMSG", "|  \\ /  \\ /  \\ |__) |__) |__  |    |");
	log_msg("Bell::bootMSG", "|__/ \\__/ \\__/ |  \\ |__) |___ |___ |___");
	log_msg("Bell::bootMSG", "");
	log_msg("Bell::bootMSG", "Author:\t\t\tPatrick Pedersen");
	log_msg("Bell::bootMSG", "License:\t\t\tGPLv3");
	log_msg("Bell::bootMSG", "Build date:\t\t" + String(__DATE__));
	log_msg("Bell::bootMSG", "Software Revision:\t" + String(SW_REV) + "_BELL");
	log_msg("Bell::bootMSG", "Hardware Revision:\t" + String(HW_REV) + "_BELL");
	log_msg("Bell::bootMSG", "Source code:\t\thttps://github.com/TU-DO-Makerspace/Wireless-Doorbell");
	log_msg("Bell::bootMSG", "Device type:\t\tBell");
	log_msg("Bell::bootMSG", "Targeted SSID:\t\t" + String(WIFI_SSID));
	log_msg("Bell::bootMSG", "---------------------------------------------------------------------------");
	log_msg("Bell::bootMSG", "");
}

// Refer to header for documentation
Bell::bell_state Bell::init()
{
	bootMSG();
	wifi_handler.connect();
	ring_receiver->begin(cfg.port, cfg.door_ip);
	return DISCONNECTED;
}

// Refer to header for documentation
Bell::bell_state Bell::disconnected()
{
	led.mode(StatusLED::BLINK);
	led.setBlinkInterval(BELL_LED_CONNECTING_BLINK_INTERVAL);
	return CONNECTING;
}

// Refer to header for documentation
Bell::bell_state Bell::connecting()
{
	if (wifi_handler.status() == WiFiHandler::CONNECTED) {
		led.mode(StatusLED::OFF);
		return CONNECTED;
	}

	return CONNECTING;
}

// Refer to header for documentation
Bell::bell_state Bell::connected()
{
	// Calling received() will reset the return value
	// to false after each call. That way we don't  keep 
	// entering this condition when a ring has been received.
	if (ring_receiver->received()) {
		led.mode(StatusLED::ON);
		buzzer.ring();
		return RINGING;
	}

	if (wifi_handler.status() == WiFiHandler::DISCONNECTED)
		return DISCONNECTED;

	return CONNECTED;
}

// Refer to header for documentation
Bell::bell_state Bell::ringing()
{
	if (!buzzer.ringing()) {
		led.mode(StatusLED::OFF);
		return CONNECTED;
	}

	return RINGING;
}

// Refer to header for documentation
Bell::bell_state Bell::error()
{
	switch (err) {
		case UNINITIALIZED: {
			// update() method called on object that hasn't been 
			// properly initialized/configured. This can occur if 
			// the object is only initialized with the default constructor.
			// Since no LED pin is initialized, we can't display error codes
			// and must resort to a fallback error.
			log_msg("Bell::error", "Bell not initialized!");
			FALLBACK_ERROR(); // Enters infinite loop
			break;
		}
		case CFG_INVALID: {
			log_msg("Bell::error", "Invalid configuration provided!");
			led.setBlinkInterval(250);
			led.mode(StatusLED::BLINK);
			break;
		}
	}

	return ERROR_HANDLING;
}

// Refer to header for documentation
Bell::bell_state Bell::error_handling()
{
	switch(err) {
		case UNINITIALIZED: 	break;	// This should never occur but keeps the compiler happy
		case CFG_INVALID: 	break;	// Blinks led, nothing to handle (taken care of by StatusLED.update())
	}

	return ERROR_HANDLING;
}

// Refer to header for documentation
void Bell::run()
{
	// The main state machine
	// Transitions are handled through return values of each state function
	switch (state) {
		case INIT:              state = init();                 break;
		case DISCONNECTED:      state = disconnected();         break;
		case CONNECTING:        state = connecting();           break;
		case CONNECTED:         state = connected();            break;
		case RINGING:           state = ringing();              break;
		case ERROR:             state = error();                break;
		case ERROR_HANDLING:    state = error_handling();       break;
	}

	// Update asynchronus components here
	wifi_handler.update();
	buzzer.update();
	led.update();
}

#endif