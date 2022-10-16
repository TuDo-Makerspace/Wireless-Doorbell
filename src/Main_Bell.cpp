#ifdef TARGET_DEV_BELL

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>

#include <log.h>
#include <ring_msg.h>
#include <Bell.h>
#include <StatusLED.h>

#include "config.h"

static IPAddress door_ip;
static AsyncClient *door_client;
static Bell bell = Bell(BELL_BUZZER, BELL_LED, BELL_MELODY, MELODY_LEN(BELL_MELODY));
static StatusLED led = StatusLED(BELL_LED);


static void handleError(void* arg, AsyncClient* client, int8_t error)
{
	log_msg("handleError", "Connection error " + String(client->errorToString(error)) + " from client " + client->remoteIP().toString());
}

static void handleData(void* arg, AsyncClient* client, void *data, size_t len)
{
	uint8_t msg;

	if (len != 1)
		goto INVALID_PACKET;

	msg = *((uint8_t *)data);

	if (msg == RING_MSG)
		log_msg("handleData", "Ring message received!");
	else
		goto INVALID_PACKET;

	client->close();
	bell.ring();

	return;

	INVALID_PACKET:
		log_msg("handleData", "Invalid packet received");
		client->close();
}

static void handleDisconnect(void* arg, AsyncClient* client)
{
	log_msg("handleDisconnect", "Client disconnected");
}

static void handleTimeOut(void* arg, AsyncClient* client, uint32_t time)
{
	log_msg("handleTimeOut", "Client timed out");
}


/* server events */
static void handleNewClient(void* arg, AsyncClient* client)
{
	// Check if IP matches door
	IPAddress client_ip = client->remoteIP();

	log_msg("handleNewClient", "New client connected with IP: " + client_ip.toString());

#ifndef DEBUG
	if (client_ip != door_ip) {
		log_msg("handleNewClient", "Client IP does not match door IP, ignoring connection");
		client->close();
		return;
	}
#endif
	
	door_client = client;
	
	// register events
	client->onData(&handleData, NULL);
	client->onError(&handleError, NULL);
	client->onDisconnect(&handleDisconnect, NULL);
	client->onTimeout(&handleTimeOut, NULL);
}

void boot_msg()
{
        log_msg("boot_msg", "---------------------------------------------------------------------------");
        log_msg("boot_msg", "___       __   __"); 
        log_msg("boot_msg", " |  |  | |  \\ /  \\");
        log_msg("boot_msg", " |  \\__/ |__/ \\__/");                
        log_msg("boot_msg", "");                                     
        log_msg("boot_msg", " __   __   __   __   __   ___");     
        log_msg("boot_msg", "|  \\ /  \\ /  \\ |__) |__) |__  |    |");
        log_msg("boot_msg", "|__/ \\__/ \\__/ |  \\ |__) |___ |___ |___");
        log_msg("boot_msg", "");
        log_msg("boot_msg", "Author:\t\tPatrick Pedersen");
        log_msg("boot_msg", "License:\t\tGPLv3");
        log_msg("boot_msg", "Build date:\t\t" + String(__DATE__));
        log_msg("boot_msg", "Software Revision:\t" + String(SW_REV) + "_BELL");
        log_msg("boot_msg", "Hardware Revision:\t" + String(HW_REV) + "_BELL");
        log_msg("boot_msg", "Source code:\t\thttps://github.com/TU-DO-Makerspace/Wireless-Doorbell");
        log_msg("boot_msg", "Device type:\t\tBell");
        log_msg("boot_msg", "Targeted SSID:\t" + String(WIFI_SSID));
        log_msg("boot_msg", "---------------------------------------------------------------------------");
        log_msg("boot_msg", "");
}

void setup()
{
	Serial.begin(115200);
	delay(500);
	Serial.println();
	boot_msg();

	const String door_ip_str = DOOR_IP;
	
	if (!door_ip.fromString(door_ip_str)) {
		log_msg("Setup", "FATAL: Invalid door IP provided, exiting");
		return;
	}

        WiFi.begin(WIFI_SSID, WIFI_PSK);

	IPAddress ip;
	ip.fromString(BELL_IP);

	IPAddress gateway;
	gateway.fromString(GATEWAY); 

	IPAddress subnet(255,255,255,0); 

	WiFi.config(ip, gateway, subnet);

	AsyncServer* server = new AsyncServer(TCP_PORT);
	server->onClient(&handleNewClient, server);
	server->begin();
}

void on_connect()
{
	log_msg("on_connect", "Connected to " + String(WIFI_SSID));
	log_msg("on_connect", "IP address: " + WiFi.localIP().toString());

	led.mode(OFF);
	led.update();
}

void on_not_connect(bool init = false)
{
	if (init)
		log_msg("on_not_connect", "Attempting to establish connection with " + String(WIFI_SSID));
	else
		log_msg("on_not_connect", "Lost connection to " + String(WIFI_SSID) + ", retrying...");

	led.setBlinkInterval(BELL_LED_CONNECTING_BLINK_INTERVAL);
	led.mode(BLINK);
}

void while_not_connected()
{
	led.update();

}

void while_connected()
{
	led.update();
}

void wifi_monitor()
{
	uint8_t con = WiFi.status();
	static int prev_con = -1;

	if (con == WL_CONNECTED) {
		if (con != prev_con)
			on_connect();

		while_connected();
	} else {
		if (con != prev_con)
			on_not_connect(prev_con == -1);

		while_not_connected();
	}
}

void loop()
{	
	wifi_monitor(); // TODO: Turn this into a class
	bell.update();
}

#endif