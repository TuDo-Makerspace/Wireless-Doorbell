#ifdef TARGET_DEV_BELL

#include <config.h>

#include <log.h>
#include <StatusLED.h>

#include <bell/fallback_error.h>
#include <bell/Bell.h>

Bell::Bell()
{
        err = UNINITIALIZED;
        state = ERROR;
}

Bell::Bell(BellCFG bell_cfg) : cfg(bell_cfg)
{
        log_msg("Bell::Bell", "Initializing bell");

        if (!cfg.checkValidity()) {
                if (cfg.led_pin == -1) {
                        log_msg("Bell::Bell", "LED indicator uninitialized, entering fallback error mode");
                        FALLBACK_ERROR(); // Enters infinite loop
                }

                err = CFG_INVALID;
                state = ERROR;
                return;
        }

        led = StatusLED(cfg.led_pin);
        buzzer = Buzzer(cfg.buzzer_pin, BELL_MELODY, MELODY_LEN(BELL_MELODY));

        IPAddress ip, gateway, subnet;
	ip.fromString(cfg.static_ip);
	gateway.fromString(cfg.gateway);
	subnet.fromString(cfg.subnet);

	wifi_handler = WiFiHandler(
		cfg.ssid, cfg.psk,
		ip, gateway, subnet,
		0
	);

        ring_receiver = RingReceiver::get_instance();

        err = NO_ERROR;
        state = INIT;
}

void Bell::bootMSG()
{
        log_msg("Bell::bootMSG", "---------------------------------------------------------------------------");
        log_msg("Bell::bootMSG", "___       __   __"); 
        log_msg("Bell::bootMSG", " |  |  | |  \\ /  \\");
        log_msg("Bell::bootMSG", " |  \\__/ |__/ \\__/");                
        log_msg("Bell::bootMSG", "");                                     
        log_msg("Bell::bootMSG", " __   __   __   __   __   ___");     
        log_msg("Bell::bootMSG", "|  \\ /  \\ /  \\ |__) |__) |__  |    |");
        log_msg("Bell::bootMSG", "|__/ \\__/ \\__/ |  \\ |__) |___ |___ |___");
        log_msg("Bell::bootMSG", "");
        log_msg("Bell::bootMSG", "Author:\t\t\tPatrick Pedersen");
        log_msg("Bell::bootMSG", "License:\t\t\tGPLv3");
        log_msg("Bell::bootMSG", "Build date:\t\t" + String(__DATE__));
        log_msg("Bell::bootMSG", "Software Revision:\t" + String(SW_REV) + "_BELL");
        log_msg("Bell::bootMSG", "Hardware Revision:\t" + String(HW_REV) + "_BELL");
        log_msg("Bell::bootMSG", "Source code:\t\thttps://github.com/TU-DO-Makerspace/Wireless-Doorbell");
        log_msg("Bell::bootMSG", "Device type:\t\tBell");
        log_msg("Bell::bootMSG", "Targeted SSID:\t\t" + String(WIFI_SSID));
        log_msg("Bell::bootMSG", "---------------------------------------------------------------------------");
        log_msg("Bell::bootMSG", "");
}

Bell::bell_state Bell::init()
{
        bootMSG();
        wifi_handler.connect();
        ring_receiver->begin(cfg.port, cfg.door_ip);
        return DISCONNECTED;
}

Bell::bell_state Bell::disconnected()
{
        led.mode(StatusLED::BLINK);
        led.setBlinkInterval(BELL_LED_CONNECTING_BLINK_INTERVAL);
        return CONNECTING;
}

Bell::bell_state Bell::connecting()
{
        if (wifi_handler.status() == WiFiHandler::CONNECTED) {
                led.mode(StatusLED::OFF);
                return CONNECTED;
        }

        return CONNECTING;
}

Bell::bell_state Bell::connected()
{
        if (ring_receiver->received()) {
                led.mode(StatusLED::ON);
                buzzer.ring();
                return RINGING;
        }

        if (wifi_handler.status() == WiFiHandler::DISCONNECTED)
                return DISCONNECTED;

        return CONNECTED;
}

Bell::bell_state Bell::ringing()
{
        if (!buzzer.ringing()) {
                led.mode(StatusLED::OFF);
                return CONNECTED;
        }

        return RINGING;
}

Bell::bell_state Bell::error()
{
        switch (err) {
                case UNINITIALIZED: {
                        log_msg("Bell::error", "Bell not initialized!");
                        FALLBACK_ERROR(); // Enters infinite loop
                        break;
                }
                case CFG_INVALID: {
                        log_msg("Bell::error", "Invalid configuration provided!");
                        led.setBlinkInterval(250);
                        led.mode(StatusLED::BLINK);
                        break;
                }
                default: {
                        log_msg("Bell::error", "Unhandled error received:" + String(err));
                }
        }

        return ERROR_HANDLING;
}

Bell::bell_state Bell::error_handling()
{
        switch(err) {
                case CFG_INVALID: break;
                default: break;
        }

        return ERROR_HANDLING;
}

void Bell::run()
{
	switch (state) {
		case INIT: {
                        state = init();
                        break;
		}
		case DISCONNECTED: {
                        state = disconnected();
                        break;
                }
                case CONNECTING: {
                        state = connecting();
                        break;
                }
                case CONNECTED: {
                        state = connected();
                        break;
                }
                case RINGING: {
                        state = ringing();
                        break;
                }
                case ERROR: {
                        state = error();
                        break;
                }
                case ERROR_HANDLING: {
                        state = error_handling();
                        break;
                }
	}

        wifi_handler.update();
        buzzer.update();
        led.update();
}

#endif