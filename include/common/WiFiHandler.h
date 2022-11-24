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
 * @file WiFiHandler.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief Provides a class for handling WiFi connections
 */

#pragma once

#include <ESP8266WiFi.h>

#define NO_TIMEOUT 0

/**
 * @brief WiFihandler class
 * 
 * The WiFiHandler class provides a simple interface for 
 * connecting to a WiFi network, as well as checking the
 * connection status.
 * 
 * The ESP8266WiFi library is used for the actual WiFi
 * connection and the state machine simply wraps around
 * the WiFi object.
 */
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
	bool rejoin;

	wifi_stat stat = DISCONNECTED;
	unsigned long timeout_tstamp;

	/**
	 * @brief Returns if the connection attempt has timed out
	 */
	bool timeout();

	/**
	 * @brief Connects to the WiFi network
	 * 
	 * The following function begins the WiFi object
	 * without setting a timeout.
	 * 
	 * Once called, the state machine is set to the
	 * CONNECTING state.
	 * 
	 * Once the connection is established, the state
	 * machine is set to the CONNECTED state.
	 */
	void _connect();

	/**
	 * @brief Reports unexpected WiFi disconnects
	 * 
	 * The following function reports unexpected WiFi
	 * disconnects. Once called, the state machine is
	 * set to the DISCONNECTED state.
	 * 
	 * If the rejoin flag is set true, the WiFi object's
	 * will disconnect() method not not be called, and a
	 * reconnect will be attempted automatically
	 * 
	 * If the rejoing flag is set false, the function redirects
	 * to the disconnect() function, which will not attempt
	 * to automatically re-establish a connection.
	 */	
	void unexpected_disconnect();

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
	WiFiHandler();
	
	/**
	 * @brief Constructor
	 * @param ssid The SSID of the WiFi network to connect to
	 * @param psk The PSK of the WiFi network to connect to
	 * @param ip The IP address to assign to the ESP8266
	 * @param gateway The gateway to assign to the ESP8266
	 * @param subnet The subnet to assign to the ESP8266
	 * @param timeout_ms The timeout in milliseconds for the connection attempt (0 = No/Infinite timeout)
	 * @param rejoin If true, the ESP8266 will attempt to reconnect automatically after unexpected disconnects
	 */
	WiFiHandler(const String ssid, const String psk, 
		    const IPAddress ip, const IPAddress gateway, const IPAddress subnet,
		    const uint16_t timeout_s, const bool rejoin = true);

	/**
	 * @brief Connects to the WiFi network
	 * 
	 * The following function should be called to connect
	 * to the WiFi network.
	 * 
	 * Once called, the begin() method of the WiFi object is called and
	 * the state machine is set to the CONNECTING state.
	 * 
	 * Once the connection is established, the state
	 * machine is set to the CONNECTED state.
	 * 
	 * If a timeout has been set and a connection could
	 * not be established until the timeout has expired,
	 * the state machine is set to the DISCONNECTED.
	 * 
	 */
	void connect();

	/**
	 * @brief Disconnects from WiFi
	 * 
	 * The following function disconnects from the WiFi
	 * network and sets the state machine to the DISCONNECTED
	 */	
	void disconnect();

	/**
	 * @brief Updates the state machine
	 * 
	 * The following function updates the state machine
	 * and must be called periodically.
	 */
	void update();

	/**
	 * @brief Returns the current state of the state machine
	 *
	 * The following function returns the current state
	 * 
	 * Possible states are:
	 * 	UNINITIALIZED: The object has not been initialized (ie. only the default constructor has been called)
	 * 	DISCONNECTED: The device is disconnected from the WiFi network
	 * 	CONNECTING: The device is attempting to connect to the WiFi network
	 * 	CONNECTED: The device is connected to the WiFi network
	 * 
	 * Use this method to check the connection status.
	 * 
	 * @returns The current state of the state machine
	 */
	wifi_stat status();
};