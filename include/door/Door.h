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
 * @file Door.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief The Main door class
 */


#pragma once

#include <ESP8266WiFi.h>

#include <StatusLED.h>
#include <WiFiHandler.h>

#include <door/DoorCFG.h>
#include <door/RingSender.h>

/**
 * @brief The Main Door class
 * 
 * The Door class handles the main logic of the Door transmitter.
 * This includes  initializing and handling all sub classes (RingSender,
 * WiFiHandler, Status LEDs etc.), printing the boot message, handling errors 
 * and more...
 * 
 * The class is initialized by taking a DoorCFG object in its constructor.
 */
class Door {
	/// State machine states
	enum door_state {
		INIT,		///< Initialization
		CONNECTING,	///< Connecting to WiFi
		CONNECTED,	///< Connected to WiFi
		RINGING,	///< Send ring message
		ERROR,		///< Error occured
		ERROR_HANDLING,	///< Error being handled
		POWER_OFF,	///< Power off
		POWERED_OFF,	///< Wait for door to power off
	};

	// Possible errors
	enum error_type {
		UNINITIALIZED,	 ///< Class only initialized with default constructor
		CFG_INVALID,	 ///< Configuration invalid
		NO_WIFI,	 ///< WiFi not available
		PARTIAL_SUCCESS, ///< Partial success (not all bells rang)
		FAIL,	 	 ///< Failure (no bells rang)
	};

	DoorCFG cfg;

	StatusLED ring_led;
	StatusLED pwr_led;

	WiFiHandler wifi_handler;
	RingSender ring_sender;

	door_state state;
	error_type err;
	
	/**
	 * @brief Prints the boot message
	 */
	void bootMSG();


	/**
	 * @brief Entry point of the state machine
	 * 
	 * The init function is the entry point of the state machine.
	 * It enables the power LED, prints the boot message, and tells 
	 * the WiFiHandler to connect to the WiFi network.
	 * 
	 * Once complete, the init method returns the CONNECTING state,
	 * which the state machine will then enter.
	 * 
	 * @returns CONNECTING, the next state of the state machine.
	 */
	door_state init();

	/**
	 * @brief Handles the CONNECTING state
	 * 
	 * The connecting() function handles the CONNECTING state.
	 * The CONNECTING state will poll the WiFiHandler to check
	 * if the connection to the WiFi network has been established.
	 * 
	 * If the connection has been established, the connecting()
	 * function will return the CONNECTED state.
	 * 
	 * If the connection attempt has timed out, the connecting()
	 * function will return the ERROR state and set the error type
	 * to NO_WIFI.
	 * 
	 * @returns The next state of the state machine:
	 * 	    CONNECTING, if the WiFiHandler is still connecting.
	 *  	    CONNECTED, if the WiFiHandler has connected.
	 * 	    ERROR (NO_WIFI), if the WiFiHandler has timed out.
	 */
	door_state connecting();

	/**
	 * @brief Handles the CONNECTED state
	 * 
	 * The connected() function handles the CONNECTED state.
	 * The CONNECTED state sends a ring message to all bells
	 * via the RingSender class and then returns the RINGING
	 * state.
	 * 
	 * @returns RIGHTING, the next state of the state machine.
	 */
	door_state connected();
	
	/**
	 * @brief Handles the RINGING state
	 * 
	 * If the RingSender is still awaiting a response, the RIGHTING
	 * state will be returned.
	 * 
	 * If all bells have been rang, the connected() function will
	 * return the POWER_OFF state.
	 * 
	 * If some bells have been rang, but not all, the connected()
	 * function will return the ERROR state and set the error type
	 * to PARTIAL_SUCCESS.
	 * 
	 * If no bells have been rang, the connected() function will
	 * return the ERROR state and set the error type to FAIL.
	 * 
	 * @returns The next state of the state machine:
	 * 	    RINGING, if the RingSender is still awaiting a response.
	 * 	    POWER_OFF, if all bells have been rang.
	 * 	    ERROR (PARTIAL_SUCCESS), if some bells have been rang, but not all.
	 * 	    ERROR (FAIL), if no bells have been rang.
	 */
	door_state ringing();
	
	/**
	 * @brief Handles the ERROR state
	 * 
	 * The error() function handles the ERROR state.
	 * If an error occurs, the state machine transitions to the
	 * ERROR state. The ERROR state determines the type of error
	 * and prepares the error handling (eg. setting status LEDs to blink mode).
	 * 
	 * The error is then handled in the error_handling() function.
	 * 
	 * @returns ERROR_HANDLING, the next state of the state machine.
	 */
	door_state error();

	/**
	 * @brief Handles the ERROR_HANDLING state
	 * 
	 * The error_handling() function handles the ERROR_HANDLING state.
	 * The ERROR_HANDLING state handles the error by blinking the LED
	 * and waiting for the user to reset the device.
	 * 
	 * @returns ERROR_HANDLING, if the error is still being handled or is fatal.
	 * 	    POWER_OFF, once the error has been handled.
	 */
	door_state error_handling();

	/**
	 * @brief Handles the POWER_OFF state
	 * 
	 * The power_off() function handles the POWER_OFF state.
	 * The POWER_OFF state unlatches the power latch and returns
	 * the POWERED_OFF state.
	 * 
	 * Ideally the POWER_OFF state should never be reached, as the
	 * device should be powered off by then. However, if a devboard
	 * is used, or the power isn't immediately cut off, the POWER_OFF
	 * state will be reached, where the device reaches an infinite 
	 * loop.
	 * 
	 * @returns POWERED_OFF, the next state of the state machine.
	 */
	door_state power_off();

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
	Door();

	/**
	 * @brief Constructor
	 * 
	 * The constructor takes a DoorCFG object as an argument.
	 * The DoorCFG object contains all the configuration for
	 * the Bell class.
	 * 
	 * @param cfg DoorCFG object containing the configuration
	 */
	Door(DoorCFG cfg);

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