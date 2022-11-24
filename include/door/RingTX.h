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
 * @file RingTX.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief RingTX class
 */

#pragma once

#include <Arduino.h>
#include <ESPAsyncTCP.h>

/**
 * @brief RingTX class
 * 
 * The RingTX class is used to transmitt a ring message
 * to a single bell usign the AsyncTCP library. It further
 * checks if the transmission was successful or not.
 * 
 * This class is instanced for every bell by the RingSender class
 */
class RingTX {
public:
	/// State machine states
	enum ring_stat {
		UNINITIALIZED,	///< Only initialized with default constructor
		AWAITING,	///< Awaiting send() call
		CONNECTING,	///< Establishing TCP connection to bell
		SENDING,	///< Sending ring message
		SUCCESS,	///< Ring message successfully sent
		FAIL		///< Failed to send ring message (timeout)
	};

private:
	String ip;
	unsigned int port;
	AsyncClient client;
	unsigned long timeout;
	unsigned long tstamp;
	
	ring_stat stat = UNINITIALIZED;

	/**
	 * @brief Sends a TCP packet of the ring message
	*/
	bool txRingMSG();

	/**
	 * @brief Handles CONNECTING state
	 * 
	 * The following function handles the CONNECTING state.
	 * 
	 * If a connection to the bell is established, the SENDING
	 * state is returned.
	 * 
	 * If the connection attempt times out, the FAIL state is
	 * returned.
	 * 
	 * @return ring_stat The next state:
	 * 	- CONNECTING: Attempting to connect to bell
	 * 	- SENDING: Connection established
	 * 	- FAIL: Connection attempt timed out
	 */
	ring_stat con();
	
	/**
	 * @brief Handles SENDING state
	 * 
	 * The following function handles the SENDING state.
	 * 
	 * If the ring message was acknowledged, the SUCCESS
	 * state is returned.
	 * 
	 * If the ring message did not receive an ACK before the
	 * timeout, the FAIL state is returned.
	 * 
	 * @return ring_stat The next state:
	 * 	- SENDING: Awaiting ACK
	 * 	- SUCCESS: Ring message successfully acknowledged
	 * 	- FAIL: Ring message timed out
	 */
	ring_stat sen();

public:
	/**
	 * @brief Default constructor
	 * 
	 * The default constructor only serves to allow the class 
	 * to be declared without immidiately initializing it. This
	 * spares us from having to allocate the object on the heap via
	 * a pointer, which we generally want to avoid on embedded 
	 * systems.
	 */
	RingTX();

	/**
	 * @brief Constructor
	 * @param dest_ip The IP address of the bell to ring
	 * @param port The port of the bell to ring
	 * @param timeout The timeout in ms for the connection and transmission to succeed
	 */
	RingTX(String dest_ip, unsigned int port, unsigned long timeout_ms);

	/**
	 * @brief Sends the ring message
	 * 
	 * The following function starts starts the transmission
	 * sequence of the ring message. It puts the state machine
	 * into the CONNECTING state.
	 * 
	 * The success of the transmission can be checked by calling
	 * the status() function.
	 */
	void send();

	/**
	 * @brief Returns the current state of the state machine
	 * 
	 * The following function returns the current state of the
	 * state machine. It is used to check the success of the
	 * transmission.
	 * 
	 * The following states are possible:
	 * 	- UNINITIALIZED: Only initialized with default constructor
	 * 	- AWAITING: Awaiting send() call
	 * 	- CONNECTING: Establishing TCP connection to bell
	 * 	- SENDING: Sending ring message
	 * 	- SUCCESS: Ring message successfully sent
	 * 	- FAIL: Failed to send ring message (timeout)
	 * 
	 * @return ring_stat The current state of the state machine
	 */
	ring_stat status();

	/**
	 * @brief Updates the RingTX state machine
	 * 
	 * The following function updates the RingTX state machine.
	 * Ensure that this function is called continuously!
	 *  
	 */
	void update();
};