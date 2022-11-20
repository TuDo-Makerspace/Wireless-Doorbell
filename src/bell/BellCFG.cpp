#ifdef TARGET_DEV_BELL

#include <log.h>

#include <bell/Bell.h>

bool BellCFG::checkValidity()
{
	bool ret = true;

	if (buzzer_pin == -1) {
		log_msg("BellCFG::valid", "Buzzer pin not specified in cfg!");
		ret = false;
	}

	if (led_pin == -1) {
		log_msg("BellCFG::valid", "LED pin not specified in cfg!");
		ret = false;
	}

	if (ssid == "") {
		log_msg("BellCFG::valid", "No SSID specified in cfg!");
		ret = false;
	}

	if (door_ip == "") {
		log_msg("BellCFG::valid", "No door IP specified in cfg!");
		ret = false;
	}

	// psk is not mandatory

	if (static_ip == "") {
		log_msg("BellCFG::valid", "No static IP specified in cfg!");
		ret = false;
	}
	
	if (gateway == "") {
		log_msg("BellCFG::valid", "No gateway specified in cfg!");
		ret = false;
	}

	if (subnet == "") {
		log_msg("BellCFG::valid", "No subnet specified in cfg!");
		ret = false;
	}

	if (port == 0) {
		log_msg("BellCFG::valid", "No port specified in cfg!");
		ret = false;
	}

	return ret;
}

#endif