#ifdef TARGET_DEV_DOOR

#include <DoorUXHandler.h>
#include <log.h>

#include "config.h"

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
			bells_remaining--;
		}
	}
}

void DoorUXHandler::handleError()
{
	if (err_req == REQUEST) {
		switch(err_type) {
			case PARTIAL_SUCCESS:
				log_msg("DoorUXHandler::handleError",  String(n_bells - successfull_connections) + "/" + String(n_bells) + " could not be contacted");
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
}

bool DoorUXHandler::done()
{
	return (bells_remaining == 0 && err_req == COMPLETE);
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