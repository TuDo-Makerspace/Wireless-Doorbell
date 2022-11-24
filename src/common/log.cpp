/*
 * Copyright (C) 2022 Patrick Pedersen, TUDO Makerspace

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
 * @file log.cpp
 * @author Patrick Pedersen
 * 
 * @brief Implements functions for logging
 * 
 * The following file implements functions for logging.
 * For more information, see the header file.
 * 
 */
#include <log.h>

// Refer to header for documentation
void log_msg(String category, String message)
{
        Serial.println("[" + String(millis())+ "]\t\t" + category + ": " + message);
}