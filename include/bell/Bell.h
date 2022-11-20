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