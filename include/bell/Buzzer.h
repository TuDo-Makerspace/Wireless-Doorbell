#pragma once

#include <stddef.h>

#include <bell/melodies.h>

class Buzzer {
private:
	enum bzr_stat {
		UNINITIALIZED,
		IDLE,
		RINGING
	};

	uint8_t pin;
	note_t *melody;
        size_t melody_len;
	size_t i_tone;

	bzr_stat stat;
	unsigned long tstamp;

public:
	Buzzer();
	Buzzer(uint8_t pin, const note_t mel[], size_t melody_len);
	void ring();
	bool ringing();
	void update();
};