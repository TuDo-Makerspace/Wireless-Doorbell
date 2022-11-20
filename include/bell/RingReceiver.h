#pragma once

#include <ESPAsyncTCP.h>

class RingReceiver {
private:
	inline static AsyncServer *server;
	inline static AsyncClient *client;

	inline static IPAddress door_ip;
	inline static bool running;
	inline static bool recv;

	inline static RingReceiver *instance;

	RingReceiver();

	static void on_new_client(void* arg, AsyncClient* new_client);
	static void on_data(void* arg, AsyncClient* client, void *data, size_t len);
	static void on_disconnect(void* arg, AsyncClient* client);
	static void on_timeout(void* arg, AsyncClient* client, uint32_t time);
	static void on_error(void* arg, AsyncClient* client, int8_t error);

public:
	static RingReceiver *get_instance();
	
	void begin(uint16_t port, String door_ip_addr);
	bool received();
};