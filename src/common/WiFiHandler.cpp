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

/**
 * @file WiFiHandler.cpp
 * @author Patrick Pedersen
 * 
 * @brief WiFiHandler class implementation
 * 
 * The following file contains the implementation of the WiFiHandler class.
 * For more information on the class, see the header file.
 * 
 */

#include <log.h>
#include <WiFiHandler.h>

// Refer to header for documentation
WiFiHandler::WiFiHandler()
{
	stat = UNINITIALIZED;
}

// Refer to header for documentation
WiFiHandler::WiFiHandler(const String ssid, const String psk, 
			 const IPAddress ip, const IPAddress gateway, const IPAddress subnet,
			 const uint16_t timeout_s, const bool rejoin)
: ssid(ssid), psk(psk), 
  ip(ip), gateway(gateway), 
  subnet(subnet), timeout_ms(timeout_s * 1000), rejoin(rejoin)
{
	log_msg("WiFiHandler::WiFiHandler", "Initializing WiFiHandler");
	stat = DISCONNECTED;
}

// Refer to header for documentation
void WiFiHandler::_connect()
{
	if (stat == UNINITIALIZED) {
		log_msg("WiFiHandler::_connect", "WiFiHandler uninitialized, cannot connect!");
		return;
	}

	if (stat == CONNECTING) {
		log_msg("WiFiHandler::_connect", "Repeated call! Already attempting to connect!");
		return;
	}

	log_msg("WiFiHandler::_connect", "Attempting to connect to: " + ssid);

	WiFi.begin(ssid, psk);
	WiFi.config(ip, gateway, subnet);

	stat = CONNECTING;
}

// Refer to header for documentation
void WiFiHandler::connect()
{
	_connect();
	if (timeout_ms > 0) {
		timeout_tstamp = millis() + timeout_ms;
	}
}

// Refer to header for documentation
void WiFiHandler::unexpected_disconnect()
{
	if (!rejoin) {
		// If rejoin isn't enabled, treat as intentional disconnect
		disconnect();
		return;
	}

	// The ESP8266WiFiClass will automatically reconnect
	// if WiFi.disconnect() has not been called

	stat = DISCONNECTED;
	
	log_msg("WiFiHandler::unexpected_disconnect", "Attempting to reconnect to: " + ssid);
}

// Refer to header for documentation
void WiFiHandler::disconnect()
{
	// disconnect() method of WiFi class will ensure
	// it no longer attempt to automatically re-connect
	WiFi.disconnect();
	stat = DISCONNECTED;

	log_msg("WiFiHandler::disconnect", "Disconnected from: " + ssid);
}

// Refer to header for documentation
bool WiFiHandler::timeout()
{
	if (timeout_ms == NO_TIMEOUT)
		return false;

	return millis() >= timeout_tstamp;
}

// Refer to header for documentation
void WiFiHandler::update()
{
	switch(stat) {
		case CONNECTING: {
			if (WiFi.status() == WL_CONNECTED) {
				log_msg("WiFiHandler::update", "Connected to: " + ssid);

				if (WiFi.localIP() != ip) {
					log_msg("WiFiHandler::update", "IP address mismatch, attempting to reconnect");
					
					// Requires "hard" reconnect
					disconnect();
					_connect();
				}

				log_msg("WiFiHandler::update", "IP: " + WiFi.localIP().toString());
				stat = CONNECTED;
			}
			else if (timeout()) {
				log_msg("WiFiHandler::update", "Timeout after " + String(timeout_ms) + "ms!");
				log_msg("WiFiHandler::update", "Failed to establish a successful connection to: " + ssid);
				disconnect();
			}
			break;
		}
		
		case CONNECTED: {
			if (WiFi.status() != WL_CONNECTED) {
				log_msg("WiFiHandler::update", "Lost connection to: " + ssid);
				unexpected_disconnect();
			}
			break;
		}
		
		case DISCONNECTED:
			if (WiFi.status() == WL_CONNECTED) {
				log_msg("WiFiHandler::update", "Re-established connection to: " + ssid);
				stat = CONNECTED;
			}
			break;
			
		default: {
			break;
		}
	}
}

// Refer to header for documentation
WiFiHandler::wifi_stat WiFiHandler::status()
{
	return stat;
}