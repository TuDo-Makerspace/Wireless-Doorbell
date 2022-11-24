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
 * @file Buzzer.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief Buzzer class
 */

#pragma once

#include <stddef.h>

#include <bell/melodies.h>

/**
 * @brief Buzzer class
 * 
 * The Buzzer class is used to aynchronously play a
 * ring tone melody on a buzzer.
 */
class Buzzer {
private:
	/// State machine states
	enum bzr_stat {
		UNINITIALIZED,
		IDLE,
		RINGING
	};

	uint8_t pin;
	note_t *melody;
        size_t melody_len;
	size_t i_tone;

	bzr_stat stat;
	unsigned long tstamp;

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
	Buzzer();

	/**
	 * @brief Constructor
	 * 
	 * The constructor initializes the Buzzer class with a given
	 * pin and melody. See melodies.h for available melodies.
	 * 
	 * @param pin The pin to use for the buzzer
	 * @param melody The melody to play
	 * @param melody_len The length of the melody
	 */
	Buzzer(uint8_t pin, const note_t mel[], size_t melody_len);

	/**
	 * @brief Tells the Buzzer to start playing the ring tone
	 * 
	 * The following method tells the Buzzer to start playing
	 * the ring tone. It will put the state machine into the
	 * RINGING state.
	 */
	void ring();

	/**
	 * @brief Check if Buzzer is (still) playing ring tone
	 * 
	 * The following method checks if the Buzzer is still playing
	 * the ring tone. It will return true if the Buzzer is still
	 * playing the ring tone (stat == RINGING), and false otherwise.
	 * 
	 * Use this method to check if the Buzzer is done playing the
	 * ring tone.
	 * 
	 * @returns true If the Buzzer is still playing the ring tone,
	 * 	    false If the Buzzer is not playing the ring tone
	 */
	bool ringing();

	/**
	 * @brief Updates the Buzzer state machine
	 * 
	 * The following method updates the Buzzer state machine.
	 * 
	 * It must be called periodically!
	 * 
	 * In the IDLE state, the update method does nothing
	 * and simply waits for the ring() method to be called.
	 * 
	 * In the RINGING state, the update method will play the
	 * ring tone. Once the ring tone is done playing, the
	 * state machine will return to the IDLE state.
	 * 
	 */
	void update();
};