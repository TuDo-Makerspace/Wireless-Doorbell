#pragma once

#include <Arduino.h>
#include <ESPAsyncTCP.h>

class RingTX {
public:
	enum ring_stat {
		UNINITIALIZED,
		AWAITING,
		CONNECTING,
		SENDING,
		SUCCESS,
		FAIL
	};

private:
	String ip;
	unsigned int port;
	AsyncClient client;
	unsigned long timeout;
	unsigned long tstamp;
	
	ring_stat stat = UNINITIALIZED;

	ring_stat con();
	ring_stat sen();

public:
	RingTX();
	RingTX(String dest_ip, unsigned int port, unsigned long timeout_ms);

	void send();
	void update();

	bool txRingMSG();

	ring_stat status();
};