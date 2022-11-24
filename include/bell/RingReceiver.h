/*
 * Copyright (C) 2022 Patrick Pedersen, TU-DO Makerspace

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

/**
 * @file RingReceiver.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief RingReceiver class
 */

#pragma once

#include <ESPAsyncTCP.h>

/**
 * @brief RingReceiver class
 * 
 * The RingReceiver class runs a asynchronous TCP server that listens for incoming
 * connections, authenticates if the connection is the door transmitter,
 * and lastly awaits a ring message.
 * 
 * Once a ring message is received,the received() function will return true.
 * 
 * To handle incoming connections and data transfers asynchronously, the class
 * uses callbacks. Since those callbacks are static, the class has been designed
 * to be a singleton. This ensures all callbacks can access the same instance of
 * the class.
 */
class RingReceiver {
private:
	inline static AsyncServer *server;
	inline static AsyncClient *client;

	inline static IPAddress door_ip;
	inline static bool running;
	inline static bool recv;

	inline static RingReceiver *instance;

	/**
	 * @brief Default constructor
	 * 
	 * The default constructor only serves to allow the class 
	 * to be declared without immidiately initializing it. This
	 * spares us from having to allocate the object on the heap via
	 * a pointer, which we generally want to avoid on embedded 
	 * systems.
	 */
	RingReceiver();

	// Callbacks

	/**
	 * @brief Callback for new connections
	 * 
	 * This callback is called when a new connection is established.
	 * If a client is already connected, or the client is not the door,
	 * the connection is closed and ignored.
	 */
	static void on_new_client(void* arg, AsyncClient* new_client);

	/**
	 * @brief Callback for data received from client
	 * 
	 * This callback is called when data is received from the client.
	 * If the data is a ring message, the received() function will return true.
	 * If the data is invalid, the connection is closed and ignored.
	 */
	static void on_data(void* arg, AsyncClient* client, void *data, size_t len);
	
	/**
	 * @brief Callback for client disconnection
	 * 
	 * This callback is called when the client disconnects.
	 * When the door disconnects, the client pointer is
	 * set to NULL.
	 */
	static void on_disconnect(void* arg, AsyncClient* client);
	
	/**
	 * @brief Callback on client timeout
	 * 
	 * This callback is called when the client times out.
	 * When the client times out, the connection is closed.
	 */
	static void on_timeout(void* arg, AsyncClient* client, uint32_t time);
	
	/**
	 * @brief Callback on client error
	 * 
	 * This callback is called when the client encounters an error.
	 * When the client encounters an error, the connection is closed.
	 */
	static void on_error(void* arg, AsyncClient* client, int8_t error);

public:
	/**
	 * @brief Gets or creates the singleton instance
	 * 
	 * The following function returns the singleton instance of the class.
	 * If the instance does not exist, it is created.
	 */
	static RingReceiver *get_instance();
	
	/**
	 * @brief Starts the ring receiver
	 * 
	 * This function starts the ring receiver. It will listen for incoming
	 * connections on the specified port. If the receiver is already running,
	 * it will be restarted.
	 * 
	 * @param port The port to listen on
	 * @param door_ip The IP address of the door transmitter
	 */
	void begin(uint16_t port, String door_ip_addr);

	/**
	 * @brief Returns if a ring message has been received
	 * 
	 * This function returns true if a ring message has been received.
	 * Calling the function will reset the return value to false.
	 * Once called, the function will reset the received value to false,
	 * and remain false until a new ring message is received.
	 * 
	 * Poll this function to check if a ring message has been received.
	 * 
	 * @returns true if a new ring message has been received,
	 *  	    false if no new ring message has been received
	 */
	bool received();
};