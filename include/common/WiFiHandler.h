#pragma once

#include <ESP8266WiFi.h>

class WiFiHandler {
public:
	enum wifi_stat {
		UNINITIALIZED,
		DISCONNECTED,
		CONNECTING,
		CONNECTED
	};

private:
	String ssid;
	String psk;
	IPAddress ip;
	IPAddress gateway;
	IPAddress subnet;
	uint16_t timeout_ms;

	wifi_stat stat = DISCONNECTED;
	unsigned long timeout_tstamp;

	bool timeout();
	void _connect();
	
public:
	WiFiHandler();
	WiFiHandler(const String ssid, const String psk, 
		    const IPAddress ip, const IPAddress gateway, const IPAddress subnet,
		    const uint16_t timeout_s);

	void connect();
	void disconnect();
	void update();
	wifi_stat status();
};