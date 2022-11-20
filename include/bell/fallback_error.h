#pragma once

#include <Arduino.h>

#include <log.h>
#include <config.h>

inline void FALLBACK_ERROR()
{
	log_msg("FALLBACK_ERROR", "Entered fallback error mode, something went horribly wrong!");

	pinMode(BELL_LED, OUTPUT);
	pinMode(BELL_BUZZER, OUTPUT);

	while(true) {
#ifndef BELL_SILENT
		tone(BELL_BUZZER, NOTE_A4);
#endif
		digitalWrite(BELL_LED, HIGH);
		delay(250);
		noTone(BELL_BUZZER);
		digitalWrite(BELL_LED, LOW);
		delay(250);
	}
}
