#ifdef TARGET_DEV_DOOR

#include <log.h>
#include <config.h>
#include <door/DoorUXHandler.h>

DoorUXHandler::DoorUXHandler(uint8_t n_bells, uint8_t ring_led_pin, uint8_t pwr_led_pin)
: n_bells(n_bells), bells_remaining(n_bells), ring_led(ring_led_pin), pwr_led(pwr_led_pin) {
}

void DoorUXHandler::onFirstBellAck()
{
	ring_led.mode(ON);
}

void DoorUXHandler::onLastDisconnect()
{
	ring_led.mode(OFF);
	if (successfull_connections == 0) {
		err_type = FAIL;
		err_req = REQUEST;
	}
	else if (successfull_connections < n_bells) {
		err_type = PARTIAL_SUCCESS;
		err_req = REQUEST;
	} else {
		_done = true;	
	}
}

void DoorUXHandler::bellAcknowledged()
{
	if (successfull_connections == 0) {
		onFirstBellAck();
	}
	successfull_connections++;
}

void DoorUXHandler::bellDisconnected()
{
	if (bells_remaining > 0) {
		if (bells_remaining == 1) {
			onLastDisconnect();
		}
		bells_remaining--;
	}
}

void DoorUXHandler::wifiError()
{
	err_type = NO_WIFI;
	err_req = REQUEST;
}

void DoorUXHandler::handleError()
{
	if (err_req == REQUEST) {
		switch(err_type) {
			case NO_WIFI:
				log_msg("DoorUXHandler::handleError", "Failed to connect to wifi! Powering off...");
				pwr_led.mode(BLINK_INV);
				ring_led.mode(BLINK);
				break;
			case PARTIAL_SUCCESS:
				log_msg("DoorUXHandler::handleError",  String(n_bells - successfull_connections) + "/" + String(n_bells) + " bells could not be contacted");
				pwr_led.mode(BLINK);
				ring_led.mode(BLINK);
				break;
			case FAIL:
				log_msg("DoorUXHandler::handleError", "No bells found");
			default:
				pwr_led.mode(BLINK);
				break;
		}
		err_req = HANDLE;
	}

	switch(err_type) {
		case NO_WIFI:
			if (pwr_led.blinks() >= DOOR_NO_WIFI_BLINKS &&
			    ring_led.blinks() >= DOOR_NO_WIFI_BLINKS) {
				err_req = COMPLETE;
			}
			break;
		case PARTIAL_SUCCESS:
			if (pwr_led.blinks() >= DOOR_PARTIAL_SUCCESS_BLINKS &&
			    ring_led.blinks() >= DOOR_PARTIAL_SUCCESS_BLINKS) {
				err_req = COMPLETE;
			}
			break;
		case FAIL:
		default:
			if (pwr_led.blinks() >= DOOR_NO_BELLS_BLINKS) {
				err_req = COMPLETE;
			}
			break;
	}

	if (err_req == COMPLETE) {
		err_req = IDLE;
		_done = true;
	}
}

bool DoorUXHandler::done()
{
	return _done;
}

void DoorUXHandler::update()
{
	if (!init) {
		pwr_led.mode(ON);
		init = true;
	}

	if (err_req != IDLE)
		handleError();
	
	ring_led.update();
	pwr_led.update();
}

#endif