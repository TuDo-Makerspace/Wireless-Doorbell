#pragma once

#include <inttypes.h>

class PowerLatch {
	uint8_t pin;

public:
	PowerLatch(uint8_t mosfet_pin);
	void latch();
	void unlatch();
};