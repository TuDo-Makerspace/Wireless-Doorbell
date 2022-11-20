#ifdef TARGET_DEV_BELL

#include <Arduino.h>

#include <log.h>
#include <config.h>

#include <bell/Buzzer.h>

Buzzer::Buzzer()
{
	stat = UNINITIALIZED;
}

Buzzer::Buzzer(uint8_t pin, const note_t mel[], size_t melody_len) : pin(pin), melody_len(melody_len)
{
	melody = new note_t[melody_len];
        memcpy(melody, mel, melody_len * sizeof(note_t));
	pinMode(pin, OUTPUT);
	stat = IDLE;
}

void Buzzer::ring()
{
	if (stat == UNINITIALIZED) {
		log_msg("Buzzer(PIN:" + String(pin) + ")::ring", "Attempted to ring with uninitialized Buzzer!");
		return;
	}

	if (stat == RINGING) {
		log_msg("Buzzer(PIN:" + String(pin) + ")::ring", "Attempted to ring while already ringing!");
		return;
	}

	log_msg("Buzzer(PIN:" + String(pin) + ")::ring", "Ringing!");

	i_tone = 0;
	tstamp = millis() + NOTE_DURATION;
	stat = RINGING;
}

bool Buzzer::ringing()
{
	return stat == RINGING;
}

void Buzzer::update()
{
	if (stat != RINGING)
		return;

	if (millis() < tstamp)
		return;

	if (i_tone == melody_len) {
		noTone(pin);
		stat = IDLE;
		log_msg("Buzzer::update", "Done ringing!");
		return;
	}

#ifndef BELL_SILENT
	tone(pin, melody[i_tone]);
#endif

	i_tone++;
	tstamp = millis() + NOTE_DURATION;
}

#endif