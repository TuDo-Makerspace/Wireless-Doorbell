#ifdef TARGET_DEV_DOOR

#include <log.h>

#include <door/DoorCFG.h>

bool DoorCFG::checkValidity()
{
	bool ret = true;

	if (ring_led_pin == -1) {
		log_msg("DoorCFG::valid", "Ring LED pin not specified in cfg!");
		ret = false;
	}

	if (power_led_pin == -1) {
		log_msg("DoorCFG::valid", "Power LED pin not specified in cfg!");
		ret = false;
	}

	if (n_bells == 0) {
		log_msg("DoorCFG::valid", "No bells specified in cfg!");
		ret = false;
	}

	if (ssid == "") {
		log_msg("DoorCFG::valid", "No SSID specified in cfg!");
		ret = false;
	}

	if (static_ip == "") {
		log_msg("DoorCFG::valid", "No static IP specified in cfg!");
		ret = false;
	}

	if (gateway == "") {
		log_msg("DoorCFG::valid", "No gateway specified in cfg!");
		ret = false;
	}

	if (subnet == "") {
		log_msg("DoorCFG::valid", "No subnet specified in cfg!");
		ret = false;
	}

	if (port == 0) {
		log_msg("DoorCFG::valid", "No port specified in cfg!");
		ret = false;
	}

	return ret;
}

#endif