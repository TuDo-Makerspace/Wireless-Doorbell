#include <Arduino.h>

#include <PowerLatch.h>

PowerLatch::PowerLatch(uint8_t mosfet_pin) : pin(mosfet_pin) {
	pinMode(pin, OUTPUT);
}

void PowerLatch::latch() {
	digitalWrite(pin, HIGH);
}

void PowerLatch::unlatch() {
	digitalWrite(pin, LOW);
}