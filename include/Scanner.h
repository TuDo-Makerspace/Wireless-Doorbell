/*
 * Copyright (C) 2021 Patrick Pedersen, TUDO Makerspace

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

#pragma once

#include <Beacon.h>

class Scanner {
        char ssid[MAX_SSID_LEN];
        char pwd[MAX_PWD_LEN];
        
public:
        // TODO: PARAMETERIZE AP_
        Scanner(const char *ap_ssid, const char *ap_pwd);
        void start();
        bool scan();
        void stop();
};