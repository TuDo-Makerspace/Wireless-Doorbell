#include <ESP8266WiFi.h>

#include <log.h>
#include <config.h>
#include <bell/WiFiMonitor.h>

WiFiMonitor::WiFiMonitor(uint8_t led_pin, unsigned long con_blink_interval_ms) 
: led(led_pin, con_blink_interval_ms) {
}

void WiFiMonitor::onConnect()
{
	log_msg("WiFiMonitor::onConnect", "Connected to " + String(WIFI_SSID));
	log_msg("WiFiMonitor::onConnect", "IP address: " + WiFi.localIP().toString());

	led.mode(StatusLED::OFF);
	led.update();
}

void WiFiMonitor::onDisconnect()
{
	log_msg("WiFiMonitor::onDisconnect", "Lost connection to " + String(WIFI_SSID) + ", attempting to reconnect...");
	led.mode(StatusLED::BLINK);
}

void WiFiMonitor::onInit()
{
	log_msg("WiFiMonitor::onInit", "Attempting to establish connection with " + String(WIFI_SSID));
	led.mode(StatusLED::BLINK);
}

void WiFiMonitor::update()
{
	if (!init) {
		onInit();
		prev_con_stat = NOT_CONNECTED;
		init = true;
	}

	con_stat con = (WiFi.status() == WL_CONNECTED) ? CONNECTED : NOT_CONNECTED;

	if (prev_con_stat != con) {
		if (con == CONNECTED)
			onConnect();
		else
			onDisconnect();
	}

	prev_con_stat = con;
	led.update();
}