#ifdef TARGET_DEV_BELL

#include <ESP8266WiFi.h>

#include <log.h>
#include <ring_msg.h>

#include <bell/RingReceiver.h>

RingReceiver::RingReceiver()
{
	log_msg("RingReceiver::RingReceiver", "Initializing RingReceiver");

	running = false;
	recv = false;
}

RingReceiver * RingReceiver::get_instance()
{
	if (instance == NULL)
		instance = new RingReceiver();

	return instance;
}

void RingReceiver::begin(uint16_t port, String door_ip_addr)
{
	if (running) {
		log_msg("RingReceiver::begin", "RingReceiver already running! Ignoring begin request...");
		return;
	}

	door_ip.fromString(door_ip_addr);
	server = new AsyncServer(port);
	server->onClient(&on_new_client, NULL);
	server->begin();
	running = true;
	
	log_msg("RingReceiver::begin", "RingReceiver started");
}

void RingReceiver::on_new_client(void* arg, AsyncClient* new_client)
{
	IPAddress ip = new_client->remoteIP();

	log_msg("handleNewClient", "New client connected with IP: " + ip.toString());

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

	client->onData(&on_data, NULL);
	client->onDisconnect(&on_disconnect, NULL);
	client->onTimeout(&on_timeout, NULL);
	client->onError(&on_error, NULL);
}

void RingReceiver::on_data(void* arg, AsyncClient* client, void *data, size_t len)
{
	log_msg("RingReceiver::on_data", "Received data from door");

	uint8_t msg;

	if (len != 1)
		goto INVALID_PACKET;

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

void RingReceiver::on_disconnect(void* arg, AsyncClient* _client)
{
	if (_client != client)
		return;

	log_msg("RingReceiver::on_disconnect", "Door disconnected");
	client = NULL;
}

void RingReceiver::on_timeout(void* arg, AsyncClient* client, uint32_t time)
{
	log_msg("RingReceiver::on_timeout", "Client: " + client->remoteIP().toString() +  " timed out!");
	client->close();
}

void RingReceiver::on_error(void* arg, AsyncClient* client, int8_t error)
{
	log_msg("RingReceiver::on_error", "Client: " + client->remoteIP().toString() + " error: " + error);
	return;
}

bool RingReceiver::received()
{
	bool ret = recv;
	recv = false;
	return ret;
}

#endif