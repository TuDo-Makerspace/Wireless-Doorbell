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
 * @file power_latch.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief Provides macros to set the power latch circuit
 */

/* Ideally this would be a class, however, since
 * the power must be latched before the capacitor
 * charges to the P-MOS's threshold voltage, the
 * task is quite time critical. To prevent any uneceassary
 * delays (ex. through object initialization), power 
 * latching is primitively implemented through macros 
 * simply calling digitalWrite().
 * 
 * For a detailed overview of the power latch circuit, please
 * refer to the kicad schematic.
 */

#include <Arduino.h>

#include <config.h>

/**
 * @brief Latches the power
 * 
 * The following function latches the power by
 * enabling the N-MOS responsible for pulling down 
 * the P-MOS gate (which sources the current for 
 * the ESP).
 * 
 * Please call this function as early as possible
 * upon boot, as the power latch circuit relies on
 * a discharged RC circuit and is thus time critical.
 */
#define LATCH_POWER() pinMode(DOOR_POWER_LATCH, OUTPUT); digitalWrite(DOOR_POWER_LATCH, HIGH)

/**
 * @brief Unlatches the power, thus powering off the device
 * 
 * This function unlatches the power by disabling the
 * Note that this will have no effect on USB powered
 * devices (for example on the devboards).
 */
#define UNLATCH_POWER() digitalWrite(DOOR_POWER_LATCH, LOW)