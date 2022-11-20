#ifdef TARGET_DEV_BELL

#include <log.h>
#include <config.h>

#include <bell/Bell.h>

Bell bell;

void setup()
{
	Serial.begin(115200);

	BellCFG cfg;

	cfg.buzzer_pin		= BELL_BUZZER;
	cfg.led_pin		= BELL_LED;
	cfg.ssid 		= WIFI_SSID;
	cfg.psk 		= WIFI_PSK;
	cfg.door_ip 		= DOOR_IP;
	cfg.static_ip 		= BELL_IP;
	cfg.gateway 		= GATEWAY;
	cfg.subnet 		= "255.255.255.0";
	cfg.port 		= TCP_PORT;

	bell = Bell(cfg);
}

void loop()
{
	bell.run();
}

#endif