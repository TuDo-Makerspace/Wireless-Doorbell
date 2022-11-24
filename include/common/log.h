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
 * @file log.h
 * @author Patrick Pedersen, TU-DO Makerspace
 * @brief Provides functions and macros for logging
 */

#include <Arduino.h>

/**
 * @brief Outputs a log message to the serial port
 * 
 * @param category The category of the log message
 * @param message The log message
 */
void log_msg(String category, String message);

/**
 * Outputs a log message to the serial port on debug builds only
 */
#ifdef DEBUG
#define DBG_LOG(CATEGORY, MSG) log_msg(CATEGORY, MSG)
#else
#define DBG_LOG(CATEGORY, MSG)
#endif
