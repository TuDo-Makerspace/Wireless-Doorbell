#pragma once

#include <inttypes.h>

#include <door/RingTX.h>

class RingSender {
public:
	enum ring_stat {
		UNINITIALIZED,
		AWAITING,
		SENDING,
		SUCCESS,
		PARTIAL_SUCCESS,
		FAIL
	};

private:
	uint8_t n_bells;
	RingTX* tx;

	ring_stat stat;

public:
	RingSender();
	RingSender(IPAddress door_ip, unsigned int port, uint8_t n_bells, unsigned long timeout_ms);
	~RingSender();

	RingSender& operator=(const RingSender& other);

	uint8_t acks();
	uint8_t fails();

	void send();
	void update();
	
	ring_stat status();
};