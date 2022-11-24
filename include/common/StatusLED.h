/*
 * Copyright (C) 2022 Patrick Pedersen, TU-DO Makerspace

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

/**
 * @file StatusLED.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief Provides a class for controlling status LEDs
 */

#pragma once

#include <inttypes.h>

/**
 * @brief StatusLED class
 * 
 * The StatusLED class provides a simple "asynchronous" interface for controlling status LEDs.
 * LEDs can be set to blink, be on or off. In order to work "asynchronously", the class
 * requires the user to continously the update() method.
 */
class StatusLED {
public:
        /// Possible LED states
        enum led_mode {
                OFF,      ///< Constantly off
                ON,       ///< Constantly on
                BLINK,    ///< First ON then OFF
                BLINK_INV ///< First OFF then ON
        };

private:
        uint8_t pin;
        bool stat;
        led_mode mod;
        unsigned long blink_interval;
        unsigned long tstamp;
        unsigned int blks;
public:
        /**
	 * @brief Default constructor
	 * 
	 * The default constructor only serves to allow the class 
	 * to be declared without immidiately initializing it. This
	 * spares us from having to allocate the object on the heap via
	 * a pointer, which we generally want to avoid on embedded 
	 * systems.
	 */
        StatusLED();

        /**
         * @brief Constructor
         * 
         * The constructor initializes the StatusLED object with the
         * given pin and blink interval.
         * 
         * The blink interval defines the time between two consecutive
         * LED state changes if the LED is set to blink.
         * 
         * @param pin The pin to which the LED is connected
         * @param blink_interval The blink interval in milliseconds (default: 500)
         */
        StatusLED(uint8_t pin, unsigned long blink_interval = 500);

        /**
         * @brief Sets the LEDs blink interval in milliseconds
         */
        void setBlinkInterval(unsigned long interval);
 
        /**
         * @brief Sets the LEDs mode
         * 
         * The following function sets the LEDs mode.
         * Available modes are:
         *      - OFF           LED is off
         *      - ON            LED is on
         *      - BLINK         LED blinks ON, then OFF
         *      - BLINK_INV     LED blinks OFF, then ON
         * 
         * The BLINK_INV mode is useful for alternating blinks between two LEDs.
         * 
         * @param mode The LEDs mode
         */
        void mode(led_mode m);

        /**
         * @brief Gets the LEDs mode
         * @returns The LEDs mode
         */
        led_mode getMode();
        
        /**
         * @brief Updates the LEDs state
         * 
         * The update() method must be called continously in order to
         * update the LEDs state. This is necessary to make the LEDs
         * blink "asynchronously".
         */        
        void update();

        /**
         * @brief Counts how many times the LED has blinked
         * 
         * The blinkCount() method returns the number of times the LED has
         * blinked since it was set to blink.
         * 
         * @returns The number of times the LED has blinked
         */
        unsigned int blinks();
};

typedef StatusLED BellLED_t;