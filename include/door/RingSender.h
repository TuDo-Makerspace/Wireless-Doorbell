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
 * @file RingSender.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief RingSender class
 */

#pragma once

#include <inttypes.h>

#include <door/RingTX.h>

/**
 * @brief The RingSender class.
 * 
 * The RingSender class is used to send a ring message to all bells,
 * as well as checking how many bells have responded.
 * 
 * It does so by creating a RingTX instance for each bell, and
 * checking if the bell has responded.
 * 
 * @attention For convenience reasons, we do not parse a fixed
 * list of bell IP addresses to the RingReceiver, but instead rely
 * on bells to reserve ip addresses following the door's ip address.
 * 
 * ```
 * Ex.:
 * Door  IP: 192.168.0.30
 * Bell1 IP: 192.168.0.31
 * Bell2 IP: 192.168.0.32
 * ...
 * BellX IP: 192.168.0.30 + X 
 * ```
 * 
 * To add bells to our network, we simply only have to update
 * the doors N_BELLS configuration parameter in the config.h file.
 */
class RingSender {
public:
	/// State machine states
	enum ring_stat {
		UNINITIALIZED,	  ///< Only initialized with default constructor
		AWAITING,	  ///< Awaiting a send() call
		SENDING,	  ///< Sending ring message
		SUCCESS,	  ///< Successfully sent ring message to all bells
		PARTIAL_SUCCESS,  ///< Successfully sent ring message to only some bells
		FAIL		  ///< Failed to contact any bells
	};

private:
	uint8_t n_bells;
	RingTX* tx;

	ring_stat stat;

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
	RingSender();
	
	/**
	 * @brief Constructor
	 * @param door_ip The IP address of the door
	 * @param port The port of the bell receivers
	 * @param n_bells The number of bells to send the ring message to
	 * @param timeout The timeout for the ring message
	 */
	RingSender(IPAddress door_ip, unsigned int port, uint8_t n_bells, unsigned long timeout_ms);
	
	/**
	 * @brief Destructor
	 * 
	 * The destructor frees any RingTX instances that have been
	 * created.
	 */
	~RingSender();

	/**
	 * @brief = operator overload
	 * 
	 * The = operator overload is used to create a deep copy
	 * of the RingSender instance.
	 */
	RingSender& operator=(const RingSender& other);

	/**
	 * @brief Number of acknowledged bells
	 * 
	 * The following function returns the number of bells that
	 * have acknowledged the ring message.
	 * 
	 * This can be used to determine how many bells have successfully
	 * received the ring message.
	 * 
	 * @returns The number of bells that have acknowledged the ring message
	 */
	uint8_t acks();

	/**
	 * @brief Number of bells that failed to acknowledge
	 * 
	 * The following function returns the number of bells that
	 * have failed to acknowledge the ring message.
	 * 
	 * A bell is considered to have failed to acknowledge the ring
	 * message if it has not responded within the timeout period.
	 */
	uint8_t fails();

	/**
	 * @brief Send ring message
	 * 
	 * The following function sends a ring message to all bells.
	 * This is done by creating a RingTX instance for each bell.
	 * 
	 * This will put the RingSender into the SENDING state.
	 */
	void send();

	/**
	 * @brief Status of the RingSender
	 * 
	 * The following function returns the current state of the
	 * RingSender.
	 * 
	 * The following states are possible:
	 * 
	 * - UNINITIALIZED: RingSender only initialized with default constructor
	 * - AWAITING: RingSender is awaiting a send() call
	 * - SENDING: RingSender is sending ring message
	 * - SUCCESS: RingSender has successfully sent ring message to all bells
	 * - PARTIAL_SUCCESS: RingSender has successfully sent ring message to only some bells
	 * - FAIL: RingSender has failed to contact any bells
	 * 
	 * Use this function to check if the RingSender has successfully or unsuccessfully
	 * sent the ring message.
	 * 
	 * @returns The current state of the RingSender
	 */
	ring_stat status();

	/**
	 * @brief Updates the RingSender state machine
	 * 
	 * The following function updates the RingSender state machine.
	 * Ensure that this function is called continuously!
	 *  
	 */
	void update();
};