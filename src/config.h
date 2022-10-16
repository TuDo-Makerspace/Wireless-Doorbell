#pragma once

#include <melodies.h>

// WiFi

// #define GATEWAY "192.168.0.1"

#ifndef GATEWAY
#warning No gateway specified! Assuming 192.168.0.1!
#define GATEWAY "192.168.0.1"
#endif

#define WIFI_SSID "TUDOMakerspace"
#define WIFI_PSK "SECRET"

// Door Host ID
#ifdef DEBUG
#define DOOR_IP "192.168.0.30"
#else
#define DOOR_IP "192.168.0.20"
#endif

// TCP
#define TCP_PORT 8888

/////////////////////////////////////
// DOOR SPECIFIC CONFIGURATION
/////////////////////////////////////

#ifdef TARGET_DEV_DOOR

#define N_BELLS 2

#if N_BELLS > 9
#error N_BELLS must be less than 10!
#endif

// Pins & Peripherals
#define DOOR_POWER_LED D1
#define DOOR_POWER_LATCH D2
#define DOOR_RING_LED D0

// Revision
#define SW_REV "2.0.0"
#define HW_REV "1.0.0"

#define DOOR_RING_CONFIRM_BLINK_INTERVAL NOTE_DURATION
#define DOOR_RING_CONFIRM_BLINKS 20
#define DOOR_NO_BELLS_BLINKS 3
#define DOOR_PARTIAL_SUCCESS_BLINKS 3

#endif

/////////////////////////////////////
// BELL SPECIFIC CONFIGURATION
/////////////////////////////////////

#ifdef TARGET_DEV_BELL

#ifndef BELL_IP
#error No IP address specified! Please define BELL_IP in the build flags (platformio.ini)!
#endif

// Revision
#define SW_REV "2.0.0"
#define HW_REV "1.0.0"

// Pins
#define BELL_LED D1
#define BELL_BUZZER D2

// Melody (See melodies.h)
#ifdef DEBUG
#define BELL_MELODY DEBUG_CHIME
// #define BELL_SILENT // Uncomment to disable bell
#elif !defined(BELL_MELODY)
#define BELL_MELODY DEFAULT_CHIME
#endif

// Indicators/Error messages
#define BELL_LED_BLINK_INTERVAL NOTE_DURATION //ms
#define BELL_LED_CONNECTING_BLINK_INTERVAL 1000 //ms

#endif // TARGET_DEV_BELL