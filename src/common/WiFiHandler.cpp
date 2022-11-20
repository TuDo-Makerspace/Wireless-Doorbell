#include <log.h>
#include <WiFiHandler.h>

WiFiHandler::WiFiHandler()
{
	stat = UNINITIALIZED;
}

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

void WiFiHandler::connect()
{
	_connect();
	if (timeout_ms > 0) {
		timeout_tstamp = millis() + timeout_ms;
	}
}

void WiFiHandler::unexpected_disconnect()
{
	if (!rejoin) {
		disconnect();
		return;
	}

	log_msg("WiFiHandler::unexpected_disconnect", "Attempting to reconnect to: " + ssid);
}

void WiFiHandler::disconnect()
{
	WiFi.disconnect();
	stat = DISCONNECTED;

	log_msg("WiFiHandler::disconnect", "Disconnected from: " + ssid);
}

bool WiFiHandler::timeout()
{
	if (timeout_ms == 0)
		return false;

	return millis() >= timeout_tstamp;
}

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
				unexpected_disconnect();
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

WiFiHandler::wifi_stat WiFiHandler::status()
{
	return stat;
}