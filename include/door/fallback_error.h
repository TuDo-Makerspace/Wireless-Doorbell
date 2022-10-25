#pragma once

#include <ESP8266WiFi.h>

#include <config.h>

#include <door/power_latch.h>

inline void FALLBACK_ERROR()
{
	log_msg("FALLBACK_ERROR", "Entered fallback error mode, something went horribly wrong!");

	pinMode(DOOR_POWER_LED, OUTPUT);
	pinMode(DOOR_RING_LED, OUTPUT);

	for (uint8_t i = 0; i < 2; i++) {
		digitalWrite(DOOR_POWER_LED, HIGH);
		delay(250);
		digitalWrite(DOOR_POWER_LED, LOW);
		delay(250);
	}

	for (uint8_t i = 0; i < 2; i++) {
		digitalWrite(DOOR_RING_LED, HIGH);
		delay(250);
		digitalWrite(DOOR_RING_LED, LOW);
		delay(250);
	}

	log_msg("FALLBACK_ERROR", "Unlatching power");

	UNLATCH_POWER();
}
