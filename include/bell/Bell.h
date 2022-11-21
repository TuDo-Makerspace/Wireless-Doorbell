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

#pragma once

#include <StatusLED.h>
#include <WiFiHandler.h>

#include <bell/BellCFG.h>
#include <bell/RingReceiver.h>
#include <bell/Buzzer.h>

class Bell {
	enum bell_state {
		INIT,
		DISCONNECTED,
		CONNECTING,
		CONNECTED,
		RINGING,
		ERROR,
		ERROR_HANDLING
	};

	enum error_type {
		NO_ERROR,
		UNINITIALIZED,
		CFG_INVALID
	};

	BellCFG cfg;

	StatusLED led;
	WiFiHandler wifi_handler;
	bell_state state;
	error_type err;
	RingReceiver *ring_receiver;
	Buzzer buzzer;

	void bootMSG();

	bell_state init();
	bell_state disconnected();
	bell_state connecting();
	bell_state connected();
	bell_state ringing();
	bell_state error();
	bell_state error_handling();

public:
	Bell();
	Bell(BellCFG bell_cfg);
	void run();
};