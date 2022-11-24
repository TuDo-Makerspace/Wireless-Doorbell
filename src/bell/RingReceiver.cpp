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
 * @file RingReceiver.cpp
 * @author Patrick Pedersen
 * 
 * @brief RingReceiver class implementation
 * 
 * The following file contains the implementation of the RingReceiver class.
 * For more information on the class, see the header file.
 * 
 */

#ifdef TARGET_DEV_BELL

#include <ESP8266WiFi.h>

#include <log.h>
#include <ring_msg.h>

#include <bell/RingReceiver.h>

// Refer to header for documentation
RingReceiver::RingReceiver()
{
	log_msg("RingReceiver::RingReceiver", "Initializing RingReceiver");

	running = false;
	recv = false;
}

// Refer to header for documentation
RingReceiver * RingReceiver::get_instance()
{
	if (instance == NULL)
		instance = new RingReceiver();

	return instance;
}

// Refer to header for documentation
void RingReceiver::begin(uint16_t port, String door_ip_addr)
{
	if (running) {
		log_msg("RingReceiver::begin", "RingReceiver already running! Ignoring begin request...");
		return;
	}

	door_ip.fromString(door_ip_addr);
	server = new AsyncServer(port);
	server->onClient(&on_new_client, NULL); // Register callback for new clients
	server->begin();
	running = true;
	
	log_msg("RingReceiver::begin", "RingReceiver started");
}

// Refer to header for documentation
void RingReceiver::on_new_client(void* arg, AsyncClient* new_client)
{
	IPAddress ip = new_client->remoteIP();

	log_msg("handleNewClient", "New client connected with IP: " + ip.toString());

	// Since the door is the only client we expect, we can just ignore any other clients
	// Theoretically, we should not be receiving any other clients than the door, but
	// just in case some goofball tries to connect to the bell, we'll just ignore them
	
	if (client != NULL) {
		log_msg("RingReceiver::on_new_client", "Client already connected! Ignoring new client...");
		new_client->close();
		return;
	}

	if (ip != door_ip) {
		log_msg("RingReceiver::on_new_client", "Client is not the door! Ignoring new client...");
		new_client->close();
		return;
	}

	client = new_client;

	log_msg("RingReceiver::on_new_client", "Client is the door!");

	// Register callbacks for client events
	client->onData(&on_data, NULL);
	client->onDisconnect(&on_disconnect, NULL);
	client->onTimeout(&on_timeout, NULL);
	client->onError(&on_error, NULL);
}

// Refer to header for documentation
void RingReceiver::on_data(void* arg, AsyncClient* client, void *data, size_t len)
{
	log_msg("RingReceiver::on_data", "Received data from door");

	uint8_t msg;

	if (len != 1)
		goto INVALID_PACKET;

 	// Cast data to uint8_t as the ring_msg is only 1 byte
	msg = *((uint8_t *)data);

	if (msg == RING_MSG) {
		recv = true;
		log_msg("RingReceiver::on_data", "Received ring message from door");
	} else {
		goto INVALID_PACKET;
	}

	log_msg("RingReceiver::on_data", "Closing connection with door");
	client->close();
	
	return;

	INVALID_PACKET:
		log_msg("RingReceiver::on_data", "Invalid packet received from door! Closing connection!");
		client->close();
}

// Refer to header for documentation
void RingReceiver::on_disconnect(void* arg, AsyncClient* _client)
{
	if (_client != client) // Don't care about other clients
		return;

	log_msg("RingReceiver::on_disconnect", "Door disconnected");
	client = NULL;
}

// Refer to header for documentation
void RingReceiver::on_timeout(void* arg, AsyncClient* client, uint32_t time)
{
	log_msg("RingReceiver::on_timeout", "Client: " + client->remoteIP().toString() +  " timed out!");
	client->close();
}

// Refer to header for documentation
void RingReceiver::on_error(void* arg, AsyncClient* client, int8_t error)
{
	log_msg("RingReceiver::on_error", "Client: " + client->remoteIP().toString() + " error: " + error);
	return;
}

// Refer to header for documentation
bool RingReceiver::received()
{
	bool ret = recv;
	recv = false;
	return ret;
}

#endif