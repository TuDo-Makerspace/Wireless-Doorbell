#ifdef TARGET_DEV_DOOR

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>

#include <log.h>
#include <ring_msg.h>
#include <StatusLED.h>
#include <DoorUXHandler.h>

#include "config.h"

#define SEARCHING_BELL 0x0
#define BELL_FOUND 0x1
#define BELL_NOT_FOUND 0x2

#define LATCH_POWER() digitalWrite(DOOR_POWER_LATCH, HIGH)
#define UNLATCH_POWER() digitalWrite(DOOR_POWER_LATCH, LOW)

static uint8_t bells_remaining = N_BELLS;
static uint8_t bell_found = SEARCHING_BELL;

static DoorUXHandler door_ux(N_BELLS, DOOR_RING_LED, DOOR_POWER_LED);

void onConnect(void* arg, AsyncClient* client) {
	log_msg("onConnect", "Connected to " + String(client->remoteIP().toString()));	
	
	const char msg = RING_MSG;
	client->add(&msg, sizeof(msg));
	
	if (client->send())
		log_msg("onConnect", "Sent ring message to " + String(client->remoteIP().toString()));
	else
		log_msg("onConnect", "Failed to send ring message to " + String(client->remoteIP().toString()));

	log_msg("onConnect", "Closing connection to " + String(client->remoteIP().toString()));
	client->close();

	door_ux.bellAcknowledged();
	bell_found = BELL_FOUND;
}

void onDisconnect(void* arg, AsyncClient* client) {
	bells_remaining--;

	if (bell_found == SEARCHING_BELL)
		bell_found = BELL_NOT_FOUND;

	log_msg("onDisconnect", "Disconnected from bell (" + String(bells_remaining) + " remaining)");

	door_ux.bellDisconnected();
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
        log_msg("boot_msg", "Software Revision:\t" + String(SW_REV) + "_DOOR");
        log_msg("boot_msg", "Hardware Revision:\t" + String(HW_REV) + "_DOOR");
        log_msg("boot_msg", "Source code:\t\thttps://github.com/TU-DO-Makerspace/Wireless-Doorbell");
        log_msg("boot_msg", "Device type:\t\tDoor");
        log_msg("boot_msg", "Targeted SSID:\t" + String(WIFI_SSID));
        log_msg("boot_msg", "---------------------------------------------------------------------------");
        log_msg("boot_msg", "");
}

void setup()
{
	// Latch Power
        pinMode(DOOR_POWER_LATCH, OUTPUT);
        LATCH_POWER();
        DBG_LOG("setup", "Power latched");
	door_ux.update();

	Serial.begin(115200);
	Serial.println();

#ifdef DEBUG
	boot_msg();
#endif

        WiFi.begin(WIFI_SSID, WIFI_PSK);

	IPAddress ip;
	ip.fromString(DOOR_IP);

	IPAddress gateway;
	gateway.fromString(GATEWAY); 

	IPAddress subnet(255,255,255,0); 

	WiFi.config(ip, gateway, subnet);

	log_msg("setup", "Attempting to connect to: " + String(WIFI_SSID));

	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		door_ux.wifiError();
		return;
	}

	log_msg("setup", "Connected to " + String(WIFI_SSID));
	log_msg("setup", "IP address: " + WiFi.localIP().toString());

	AsyncClient *clients[N_BELLS];
	for (uint8_t i = 0; i < N_BELLS; i++) {
		AsyncClient *client = new AsyncClient();
		client->onConnect(&onConnect, client);
		client->onDisconnect(&onDisconnect, client);
		clients[i] = client;
	}

	const String door_ip_str = DOOR_IP;

	for (uint8_t i = 0; i < N_BELLS; i++) {
		String bell_ip_str = door_ip_str;
		bell_ip_str[bell_ip_str.length() - 1] = (i+1) + '0';
		log_msg("setup", "Attempting to connect to bell: " + bell_ip_str);
		clients[i]->connect(bell_ip_str.c_str(), TCP_PORT);
	}
}

void power_off() {
	UNLATCH_POWER();

#ifdef DEBUG
	digitalWrite(DOOR_RING_LED, LOW);
	digitalWrite(DOOR_POWER_LED, LOW);
	
	static bool unlatch_logged = false;
	if (!unlatch_logged) {
		log_msg("power_off", "Power unlatched");
		unlatch_logged = true;
	}
#endif

}

void loop()
{
	if (door_ux.done()) {
		power_off();
		return;
	}

	door_ux.update();

}

#endif