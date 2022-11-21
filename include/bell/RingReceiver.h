/*
 * Copyright (C) 2022 Patrick Pedersen, TUDO Makerspace

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

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