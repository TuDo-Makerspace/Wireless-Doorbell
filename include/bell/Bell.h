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
 * @file Bell.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief Main Bell class
 */

#pragma once

#include <StatusLED.h>
#include <WiFiHandler.h>

#include <bell/BellCFG.h>
#include <bell/RingReceiver.h>
#include <bell/Buzzer.h>

/**
 * @brief The Main Bell class
 * 
 * The Bell class handles the main logic of the Bell receiver. This includes 
 * initializing and handling all sub classes (Buzzer, WiFiHandler,
 * RingReceiver, etc.), printing the boot message, handling errors 
 * and more...
 * 
 * The class is initialized by taking a BellCFG object in its constructor.
 */

class Bell {
	/// State machine states
	enum bell_state {
		INIT,		/// Initialization
		DISCONNECTED,	/// Disconnected from WiFi
		CONNECTING,	/// Connecting to WiFi
		CONNECTED,	/// Connected to WiFi
		RINGING,	/// Ringing (Ring message received)
		ERROR,		/// Error occured
		ERROR_HANDLING	/// Error being handled
	};

	// Possible errors
	enum error_type {
		UNINITIALIZED,	/// Class only initialized with default constructor
		CFG_INVALID	/// Configuration invalid
	};

	BellCFG cfg;

	StatusLED led;
	WiFiHandler wifi_handler;
	bell_state state;
	error_type err;
	RingReceiver *ring_receiver;
	Buzzer buzzer;

	/**
	 * @brief Prints the boot message
	 */
	void bootMSG();

	/**
	 * @brief Entry point of the state machine
	 * 
	 * The init function is the entry point of the state machine.
	 * It prints the boot message, tells the WiFiHandler to connect
	 * to the WiFi network and initializes the RingReceiver.
	 * 
	 * Once complete, the init method returns the DISCONNECTED state,
	 * which the state machine will then enter.
	 * 
	 * @returns DISCONNECTED, the next state of the state machine.
	 */
	bell_state init();

	/**
	 * @brief Handles the DISCONNECTED state
	 * 
	 * The disconnected() function handles the DISCONNECTED state.
	 * The DISCONNECTED state makes the LED blink and transitions
	 * the state machine to the CONNECTING state. 
	 * 
	 * The DISCONNECTED state is entered after the INIT state, or
	 * if the WiFi connection is lost in the CONNECTED state.
	 * 
	 * @returns CONNECTING, the next state of the state machine.
	 */
	bell_state disconnected();

	/**
	 * @brief Handles the CONNECTING state
	 * 
	 * The connecting() function handles the CONNECTING state.
	 * The CONNECTING state keeps the LED blinking until the
	 * WiFiHandler has successfully connected to the WiFi network.
	 * Once connected, the state machine transitions to the CONNECTED
	 * state.
	 * 
	 * The CONNECTING state is entered after the DISCONNECTED state.
	 * 
	 * @returns CONNECTING, if the WiFiHandler is still connecting.
	 *  	    CONNECTED, if the WiFiHandler has connected.
	 */
	bell_state connecting();

	/**
	 * @brief Handles the CONNECTED state
	 * 
	 * The connected() function handles the CONNECTED state.
	 * The CONNECTED state polls the RingReceiver object for
	 * new ring messages. If a new ring message is received,
	 * the state machine transitions to the RINGING state.
	 * 
	 * If the WiFi connection is lost during this state, the
	 * state machine transitions back to the DISCONNECTED state.
	 * 
	 * @returns CONNECTED, if the WiFiHandler is still connected.
	 *  	    DISCONNECTED, if the WiFiHandler has disconnected.
	 */
	bell_state connected();

	/**
	 * @brief Handles the RINGING state
	 * 
	 * The ringing() function handles the RINGING state.
	 * The RINGING state plays the ring tone melody on the buzzer
	 * and enables the LED. Once the ring tone has finished playing,
	 * the state machine transitions to the CONNECTED state.
	 * 
	 * @returns RINGING, if the ring tone is still playing.
	 * 	    CONNECTED, if the ring tone has finished playing.
	 */
	bell_state ringing();

	/**
	 * @brief Handles the ERROR state
	 * 
	 * The error() function handles the ERROR state.
	 * If an error occurs, the state machine transitions to the
	 * ERROR state. The ERROR state determines the type of error
	 * and prepares the error handling (eg. setting LED to blink mode).
	 * 
	 * The error is then handled in the error_handling() function.
	 * 
	 * @returns ERROR_HANDLING, the next state of the state machine.
	 */
	bell_state error();

	/**
	 * @brief Handles the ERROR_HANDLING state
	 * 
	 * The error_handling() function handles the ERROR_HANDLING state.
	 * The ERROR_HANDLING state handles the error by blinking the LED
	 * and waiting for the user to reset the device.
	 * 
	 * @returns ERROR_HANDLING, if the error is still being handled or is fatal.
	 * 	    Another state, if the error has been handled and isn't
	 * 	    fatal.
	 */
	bell_state error_handling();

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
	Bell();

	/**
	 * @brief Constructor
	 * 
	 * The constructor takes a BellCFG object as an argument.
	 * The BellCFG object contains all the configuration for
	 * the Bell class.
	 * 
	 * @param cfg BellCFG object containing the configuration
	 */
	Bell(BellCFG bell_cfg);

	/**
	 * @brief Runs the state machine
	 * 
	 * The following function runs the main state machine
	 * of the Bell class. It is the entry point of the class
	 * and must be continuously called in the main loop of
	 * the program.
	*/
	void run();
};